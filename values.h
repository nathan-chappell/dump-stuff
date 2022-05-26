#ifndef VALUES_H
#define VALUES_H

template <int N>
struct Int {
    static const int value = N;
};

#define FAIL 5
#define SUCCESS 6
#define INIT 7

using Fail = Int<FAIL>;
using Success = Int<SUCCESS>;
using Init = Int<INIT>;

#define _NULL 9

using Null = Int<_NULL>;
using Zero = Int<0>;
using One = Int<1>;

#define LEFT 2
#define RIGHT 3
#define STAY 4

using Left = Int<LEFT>;
using Right = Int<RIGHT>;
using Stay = Int<STAY>;


#endif