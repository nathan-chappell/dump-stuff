#ifndef DOUBLE_COUNTER_H
#define DOUBLE_COUNTER_H

#inclue "values.h"

// reads language 0^n 1^m 0^n 1^m
// states:
//  reading-zeros:
//      overwrite-left-zero
//      scan-right-for-zero-in-zeros
//      scan-right-for-zero-in-ones
//      overwrite-right-zero
//      scan-left-for-zero

template <typename State, typename Head>
struct DoubleCounterTransition {
    using newState = Halt;
    using newHead = Head;
    using direction = Stay;
}

#define TRANSITION(fromState, fromHead, toState, toHead, direction) \
template <> \
struct DoubleCounterTransition<Int< fromState >, Int< fromHead >> { \
    using newState = Int< toState >; \
    using newHead = Int< toHead >; \
    using direction = Int< direction >; \
}

TRANSITION()

#undef TRANSITION

#endif