#include <iostream>
#include <type_traits>

#include "values.h"
#include "stack.h"
#include "frame.h"

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

int main(int argc, char **argv) {
    using stack = MakeStack<1,1,1,0,0,1,1,1,0,0>;
    dump_stack<stack>();
}