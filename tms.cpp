#include <iostream>
#include <type_traits>

#include "values.h"
#include "stack.h"
#include "frame.h"
#include "double_counter.h"
#include "engine.h"

using namespace std;

template<typename Stack, enable_if_t<is_same_v<rest_t<Stack>, EmptyStack>, bool> = true>
void dump_stack() {
    cout << "| " << top_t<Stack>::value << endl;
}

template<typename Stack, enable_if_t<!is_same_v<rest_t<Stack>, EmptyStack>, bool> = true>
void dump_stack() {
    cout << "| " << top_t<Stack>::value << endl;
    dump_stack<rest_t<Stack>>();
}

template <typename Frame>
void dump_frame() {
    cout << "STATE: " << state_t<Frame>::value << endl;
    cout << "L_STACK" << endl;
    dump_stack<left_stack_t<Frame>>();
    cout << "R_STACK" << endl;
    dump_stack<right_stack_t<Frame>>();
}

template <typename RunT, enable_if_t<is_same<NullRun, RunT>::value, bool> = true>
void dump_run() {
    cout << "run dump complete" << endl;
}

template <typename RunT, enable_if_t<!is_same<NullRun, RunT>::value, bool> = true>
void dump_run() {
    dump_frame<typename RunT::nextFrame>();
    cout << "--------" << endl;
    dump_run<typename RunT::nextRun>();
}

int main(int argc, char **argv) {
    using stack = MakeStack<0,0,0,1,1,0,0,0,1,1>;
    // dump_stack<stack>();

    using run = RunTM<DoubleCounterTM, stack>;

    static_assert(run::run::finalState::value != Success::value, "The TM accepted the input");
    static_assert(run::run::finalState::value != Fail::value, "The TM did not accept the input");
    // static_assert(false, "The TM threw an exception");

    dump_run<run::run>();
    // cout << "final state: " << run::run::finalState::value << endl;
}