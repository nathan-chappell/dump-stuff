#ifndef DOUBLE_COUNTER_H
#define DOUBLE_COUNTER_H

#include "values.h"
#include "frame.h"

template <typename State, typename Head>
struct DoubleCounterTM {
    using newState = Fail;
    using newHead = Int<200>;
    using direction = Stay;
    using frameUpdate = FrameUpdate<newState, newHead, direction>; \
};

#define TRANSITION(fromState, fromHead, toState, toHead, dir) \
template <> \
struct DoubleCounterTM<Int< fromState >, Int< fromHead >> { \
    using newState = Int< toState >; \
    using newHead = Int< toHead >; \
    using direction = Int< dir >; \
    using frameUpdate = FrameUpdate<newState, newHead, direction>; \
};

// states
enum
{
    check_input__first_0s = 100,
    check_input__first_1s,
    check_input__second_0s,
    check_input__second_1s,

    init_erase_0,

    erase_0__from_right,
    erase_0__from_left,
    erase_0__check_left,
    erase_0__return_right,
    erase_0__check_0_on_right,
    erase_0__check_0_on_left,

    init_erase_1,

    erase_1__from_left,
    erase_1__from_right,
    erase_1__return_left,
    erase_1__check_1_on_right,
};

#define N 0xff
#define L 2
#define R 3
#define STAY 4

TRANSITION(INIT, 0, check_input__first_0s, 0, R);
// TRANSITION(INIT, 0, FAIL, 0, R);

TRANSITION(check_input__first_0s, 0, check_input__first_0s, 0, R);
// TRANSITION(check_input__first_0s, 1, FAIL, 1, R);
TRANSITION(check_input__first_0s, 1, check_input__first_1s, 1, R);
TRANSITION(check_input__first_1s, 1, check_input__first_1s, 1, R);
TRANSITION(check_input__first_1s, 0, check_input__second_0s, 0, R);

TRANSITION(check_input__second_0s, 0, check_input__second_0s, 0, R);
// TRANSITION(check_input__second_0s, 0, FAIL, 0, R);
TRANSITION(check_input__second_0s, 1, check_input__second_1s, 1, R);
TRANSITION(check_input__second_1s, 1, check_input__second_1s, 1, R);
TRANSITION(check_input__second_1s, N, init_erase_0, 0, L);

TRANSITION(init_erase_0, 1, init_erase_0, 1, L);
TRANSITION(init_erase_0, 0, erase_0__from_right, 0, STAY);

TRANSITION(erase_0__from_right, 0, erase_0__from_left, N, L);
TRANSITION(erase_0__from_left, 0, erase_0__from_left, 0, L);
TRANSITION(erase_0__from_left, 1, erase_0__from_left, 1, L);
TRANSITION(erase_0__from_left, N, erase_0__check_left, N, R);

TRANSITION(erase_0__check_left, 0, erase_0__return_right, N, R);
TRANSITION(erase_0__return_right, 0, erase_0__return_right, 0, R);
TRANSITION(erase_0__return_right, 1, erase_0__return_right, 1, R);
TRANSITION(erase_0__return_right, N, erase_0__check_0_on_right, N, L);

TRANSITION(erase_0__check_0_on_right, 0, erase_0__from_right, 0, STAY);
TRANSITION(erase_0__check_0_on_right, 1, erase_0__check_0_on_left, 1, L);
TRANSITION(erase_0__check_0_on_left, 1, erase_0__check_0_on_left, 1, L);
TRANSITION(erase_0__check_0_on_left, N, init_erase_1, 0, R);

TRANSITION(init_erase_1, 1, init_erase_1, 1, R);
TRANSITION(init_erase_1, N, erase_1__from_left, N, L);

TRANSITION(erase_1__from_left, 1, erase_1__from_right, N, R);
TRANSITION(erase_1__from_right, N, erase_1__from_right, N, R);
TRANSITION(erase_1__from_right, 1, erase_1__return_left, N, L);
TRANSITION(erase_1__return_left, N, erase_1__return_left, N, L);
TRANSITION(erase_1__return_left, 1, erase_1__from_left, 1, STAY);
TRANSITION(erase_1__return_left, 0, erase_1__check_1_on_right, N, R);

TRANSITION(erase_1__check_1_on_right, N, erase_1__check_1_on_right, N, R);
TRANSITION(erase_1__check_1_on_right, 0, SUCCESS, N, STAY);


#undef L
#undef R
#undef STAY
#undef TRANSITION

#endif