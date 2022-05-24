#ifndef VALUES_H
#define VALUES_H

template <int N>
struct Int {
    static const int value = N;
};

using Zero = Int<0>;
using One = Int<1>;
using Null = Int<0xff>;

using Left = Int<2>;
using Right = Int<3>;
using Stay = Int<4>;
using Halt = Int<5>;

#endif