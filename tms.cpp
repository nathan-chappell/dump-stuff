#include <iostream>
#include <iomanip>
#include <type_traits>

#include "values.h"
#include "stack.h"
#include "frame.h"
#include "double_counter.h"
#include "engine.h"

using namespace std;

template<typename Stack, enable_if_t<is_same_v<rest_t<Stack>, EmptyStack>, bool> = true>
void dump_stack(bool reverse = false, bool first = true) {
    if (!reverse && !first) {
        cout << " ";
    }
    cout << top_t<Stack>::value;
}

template<typename Stack, enable_if_t<!is_same_v<rest_t<Stack>, EmptyStack>, bool> = true>
void dump_stack(bool reverse = false, bool first = true) {
    if (reverse) {
        dump_stack<rest_t<Stack>>(reverse, false);
        cout << " " << top_t<Stack>::value;
    } else {
        if (!first) {
            cout << " ";
        }
        cout << top_t<Stack>::value;
        dump_stack<rest_t<Stack>>(reverse, false);
    }
}

template <typename Frame>
void dump_frame() {
    cout << "STATE: " << setw(4) << state_t<Frame>::value << "\t";
    dump_stack<left_stack_t<Frame>>(true);
    cout << "|";
    dump_stack<right_stack_t<Frame>>();
    cout << endl;
}

template <typename RunT, enable_if_t<is_same<NullRun, RunT>::value, bool> = true>
void dump_run() {
    cout << "run dump complete" << endl;
}

template <typename RunT, enable_if_t<!is_same<NullRun, RunT>::value, bool> = true>
void dump_run() {
    dump_frame<typename RunT::nextFrame>();
    // cout << "--------" << endl;
    dump_run<typename RunT::nextRun>();
}

int main(int argc, char **argv) {
    using stack = MakeStack<0,1,1,0,1,1>;
    // dump_stack<stack>();

    using run = RunTM<DoubleCounterTM, stack>;

    // static_assert(run::run::finalState::value != Success::value, "The TM accepted the input");
    // static_assert(run::run::finalState::value != Fail::value, "The TM did not accept the input");
    // static_assert(false, "The TM threw an exception");

    dump_run<run::run>();
    // cout << "final state: " << run::run::finalState::value << endl;
}