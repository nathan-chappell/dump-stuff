#ifndef STACK_H
#define STACK_H

#include "values.h"

struct EmptyStack {
    using top = Null;
    using rest = EmptyStack;
};

template <typename Stack>
using top_t = typename Stack::top;

template <typename Stack>
using rest_t = typename Stack::rest;

template <typename Stack, typename NewTop>
struct Push {
    using top = NewTop;
    using rest = Stack;
};

template <typename Stack>
struct Pop {
    using top = top_t<rest_t<Stack>>;
    using rest = rest_t<rest_t<Stack>>;
};

template <int Val, int ...Values>
struct MakeStack {
    using top = Int<Val>;
    using rest = EmptyStack;
};

template <int Val, int Val2, int ...Values>
struct MakeStack<Val, Val2, Values...> {
    using top = Int<Val>;
    using rest = MakeStack<Val2, Values...>;
};

#endif