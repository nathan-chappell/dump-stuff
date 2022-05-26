#ifndef VALUES_H
#define VALUES_H

template <int N>
struct Int {
    static const int value = N;
};

using Null = Int<0xff>;
using Zero = Int<0>;
using One = Int<1>;

using Left = Int<2>;
using Right = Int<3>;
using Stay = Int<4>;

#define FAIL 5
#define SUCCESS 6
#define INIT 7

using Fail = Int<FAIL>;
using Success = Int<SUCCESS>;
using Init = Int<INIT>;

#endif