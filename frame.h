#ifndef FRAME_H
#define FRAME_H

#include "stack.h"

template <typename Input>
struct InitialFrame {
    using leftStack = EmptyStack;
    using rightStack = Input;
    using state = Init;
};

template <typename Frame> using left_stack_t = typename Frame::leftStack;
template <typename Frame> using right_stack_t = typename Frame::rightStack;
template <typename Frame> using state_t = typename Frame::state;
// template <typename T> constexpr bool state_v = T::state::value;
template <typename Frame> using head_t = top_t<right_stack_t<Frame>>;

template <typename NewState, typename NewHead, typename Direction>
struct FrameUpdate {
    using newState = NewState;
    using newHead = NewHead;
    using direction = Direction;
};

template <typename FrameUpdate> using new_head_t = typename FrameUpdate::newHead;
template <typename FrameUpdate> using move_t = typename FrameUpdate::move;

template <typename Frame, typename FrameUpdate>
struct NextFrame {
    using leftStack = left_stack_t<Frame>;
    using rightStack = right_stack_t<Frame>;
    using state = Fail;
    // using leftStack = EmptyStack;
    // using rightStack = EmptyStack;
};

#define NEXT_FRAME(direction) \
template <typename Frame, typename NewState, typename NewHead> \
struct NextFrame<Frame, FrameUpdate<NewState, NewHead, direction>>

#define L_STACK left_stack_t<Frame>
#define R_STACK right_stack_t<Frame>
#define L_TOP top_t<L_STACK>

NEXT_FRAME(Stay) {
    using leftStack = L_STACK;
    using rightStack = Push< Pop<R_STACK>, NewHead>;
    using state = NewState;
};

NEXT_FRAME(Left) {
    using leftStack = Pop<L_STACK>;
    using rightStack = Push< Push< Pop<R_STACK>, NewHead>, L_TOP>;
    using state = NewState;
};

NEXT_FRAME(Right) {
    using leftStack = Push<L_STACK, NewHead>;
    using rightStack = Pop<R_STACK>;
    using state = NewState;
};

#undef L_STACK
#undef R_STACK
#undef L_TOP
#undef NEXT_FRAME

#endif