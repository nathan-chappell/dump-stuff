param(
    [Parameter(Mandatory = $true)]
    [string] $ObjFile
)

$WindowsConfig = @{
    Objdump         = 'C:\msys64\mingw64\bin\objdump.exe';
    DataSectionName = ".rdata";
    MakeLineSpaces1 = ' ' * " 14001b000 ".Length;
    MakeLineSpaces2 = ' ' * " 14001b000 7374643a 3a626164".Length;
}

$LinuxConfig = @{
    Objdump         = "objdump";
    DataSectionName = ".data.rel.ro";
    MakeLineSpaces1 = ' ' * " 470d00 ".Length;
    MakeLineSpaces2 = ' ' * " 470d00 ffff488d bda0fbff ".Length;
}

$linux = $true

if ($linux) {
    $Config = $LinuxConfig
}
else {
    $Config = $WindowsConfig
}

$print = $false;

$code = & $Config.Objdump -dCS -j .text $ObjFile | Where-Object {
    if ($_ -match '^0') {
        $print = $_ -cmatch 'Demo|[A-D]::'
    }
    $print
}

$syms = & $Config.Objdump -tC $ObjFile | Where-Object {
    $_ -match '(VTT|vtable) for [A-D]'
}

$rdata = & $Config.Objdump -s -j $Config.DataSectionName $ObjFile | Select-Object -Skip 4

function ParseSectionStart($section) {
    $line = $section | Where-Object { $_ -match '^\s*\d' } | Select-Object -First 1
    $field = $line.Trim().Split(' ')[0]
    [System.Uint64]::Parse($field, [System.Globalization.NumberStyles]::HexNumber)
}

function ParseSymWindows($line, $start) {
    if ($line -match '0x(?<Offset>[0-9a-f]+) (?<Name>.*)') {
        $offset = [System.Uint64]::Parse($Matches.Offset, [System.Globalization.NumberStyles]::HexNumber)
        @{
            Address = [System.Uint64]($offset + $start);
            Name    = $Matches.Name;
        }
    }
}

function ParseSymLinux($line) {
    if ($line -match '^(?<Address>[0-9a-f]+) .* (?<Section>[.a-z]+) ([0-9a-z]+) (?<Name>.*)$') {
        $address = [System.Uint64]::Parse($Matches.Address, [System.Globalization.NumberStyles]::HexNumber)
        @{
            Address = [System.Uint64]($address);
            Name    = $Matches.Name;
        }
    }
}


function ParseSym($line, $start) {
    if ($linux) {
        ParseSymLinux($line)
    }
    else {
        ParseSymWindows($line, $start)
    }
}

function ParseFunction($line) {
    $split = $line -split '<'
    @{
        Address = [System.UInt64]::Parse($split[0], [System.Globalization.NumberStyles]::HexNumber);
        Name    = $split[1].Substring(0, $split[1].Length - 2);
    }
}

function ParseUInt32LE($le) {
    $be = $le[6] + $le[7] + $le[4] + $le[5] + $le[2] + $le[3] + $le[0] + $le[1]
    [System.UInt32]::Parse($be, [System.Globalization.NumberStyles]::HexNumber)
}

function ParseData($line) {
    if ($line -match '^\s*(?<Address>[0-9a-f]+) (?<v1Low>[0-9a-f]+) (?<v1High>[0-9a-f]+) (?<v2Low>[0-9a-f]+) (?<v2High>[0-9a-f]+)') {
        $address = [System.UInt64]::Parse($Matches.Address, [System.Globalization.NumberStyles]::HexNumber)
        $v1Low = ParseUInt32LE $Matches.v1Low
        $v1High = ParseUInt32LE $Matches.v1High
        $v2Low = ParseUInt32LE $Matches.v2Low
        $v2High = ParseUInt32LE $Matches.v2High
        # "Data: ", $address, $v1Low, $v1High, $v2Low, $v2High, $line -join ", " | Write-Host
        @{
            Address = [System.UInt64]$address
            V1      = [System.UInt64]($v1Low + [System.UInt64]$v1High * 0x100000000)
            V2      = [System.UInt64]($v2Low + [System.UInt64]$v2High * 0x100000000)
        }
    }
}

function MakeLine($fun1, $sym1, $fun2, $sym2) {
    $line = $null
    if ($null -ne $fun1) {
        $line = $Config.MakeLineSpaces1 + $fun1
    }
    elseif ($null -ne $sym1) {
        $line = $Config.MakeLineSpaces1 + $sym1
    }
    if ($null -ne $fun2) {
        if ($null -eq $line) {
            $line = $Config.MakeLineSpaces2
        }
        $line += " " + $fun2
    }
    elseif ($null -ne $sym2) {
        if ($null -eq $line) {
            $line = $Config.MakeLineSpaces2
        }
        $line += " " + $sym2
    }
    $line
}

$rdataStart = ParseSectionStart $rdata

$FunctionsByAddress = @{}
$SymbolsByAddress = @{}

$code | Where-Object { $_ -match '^0' } | ForEach-Object { $f = ParseFunction $_; $FunctionsByAddress[$f.Address] = $f.Name }
$syms | ForEach-Object { ParseSym $_  $rdataStart } | ForEach-Object { $SymbolsByAddress[$_.Address] = $_.Name }

$print = $false
$LabeledRData = $rdata | ForEach-Object {
    $data = ParseData $_;
    if ($null -eq $data.Address) {
        # "bad-line ", $data.Address, $data.V1, $data.V2, $_ -join ", " | Write-Host;
        continue;
    }
    $fun1 = $FunctionsByAddress[$data.V1]
    $sym1 = $SymbolsByAddress[$data.Address]
    $sym1p = $SymbolsByAddress[$data.V1]
    if ($null -ne $sym1p) {
        $sym1p = "-> " + $sym1p
    }
    $sym1 ??= $sym1p
    $fun2 = $FunctionsByAddress[$data.V2]
    $sym2 = $SymbolsByAddress[$data.Address + 0x08] ?? $SymbolsByAddress[$data.V2]
    $sym2p = $SymbolsByAddress[$data.V1]
    if ($null -ne $sym2p) {
        $sym2p = "-> " + $sym2p
    }
    $sym2 ??= $sym1p
    $line = MakeLine $fun1 $sym1 $fun2 $sym2
    if ($null -ne $line) {
        $print = $true;
        $line
    }
    if ($print) {
        # $_
    }
}


# "`nCODE`n"
# $code

# "`nFunctionsByAddress`n"
# $FunctionsByAddress.Keys | ForEach-Object { [String]::Format('{0,-20:x} {1}', $_, $FunctionsByAddress[$_]) }

# "`nSymbolsByAddress`n"
# $SymbolsByAddress.Keys | ForEach-Object { [String]::Format('{0,-20:x} {1}', $_, $SymbolsByAddress[$_]) }

# "`nLabeledRData`n"
# $LabeledRData