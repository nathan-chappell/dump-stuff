#ifndef ENGINE_H
#define ENGINE_H

#include <type_traits>
#include "frame.h"

using namespace std;

struct NullRun {
    using finalState = Null;
};

template <
    bool IsDone,
    template <template <typename State, typename Head> typename TM, typename Frame> typename Engine,
    template <typename State, typename Head> typename TM,
    typename Frame
>
struct Continue {
    using nextRun = NullRun;
};

template <
    template <template <typename State, typename Head> typename TM, typename Frame> typename Engine,
    template <typename State, typename Head> typename TM,
    typename Frame
>
struct Continue<false, Engine, TM, Frame> {
    using nextRun = Engine<TM, Frame>;
};

template <template <typename State, typename Head> typename TM, typename Frame>
struct RunTMInternal {
    using frameUpdate = typename TM<state_t<Frame>, head_t<Frame>>::frameUpdate;
    using nextFrame = NextFrame<Frame, frameUpdate>;

    static const bool isDone = state_t<nextFrame>::value == Success::value || state_t<nextFrame>::value == Fail::value;

    // using finalState = state_t<nextFrame>;
    using nextRun = typename Continue<isDone, ::RunTMInternal, TM, nextFrame>::nextRun;
    using finalState = conditional_t<isDone, state_t<nextFrame>, typename nextRun::finalState>;

    // using nextRun = conditional_t<isDone, NullRun, RunTMInternal<TM, nextFrame>>;
    // using finalState = conditional_t<isDone, state_t<nextFrame>, typename nextRun::finalState>;
};

template <template <typename State, typename Head> typename TM, typename Input>
struct RunTM {
    using run = RunTMInternal<TM, InitialFrame<Input>>;
};

#endif