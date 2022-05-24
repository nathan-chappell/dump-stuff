#ifndef FRAME_H
#define FRAME_H

#include "stack.h"

template <typename LeftStack, typename RightStack, typename State>
struct FrameT {
    using leftStack = LeftStack;
    using rightStack = RightStack;
    using state = State;
};

template <typename Frame> using left_t = typename Frame::leftStack;
template <typename Frame> using right_t = typename Frame::rightStack;
template <typename Frame> using state_t = typename Frame::state;
template <typename Frame> using head_t = top_t<right_t<Frame>>;




#endif