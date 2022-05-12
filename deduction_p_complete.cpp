#include <iostream>
#include <type_traits>

using namespace std;

void print_types(...) {}

template <typename T, typename... Ts> void print_types(T, Ts...) {
  cout << typeid(T).name() << endl;
  print_types(Ts{}...);
}

struct Void {};

using One = Void *;
extern const One one;
using Zero = Void;
extern const Zero zero;

One And(One, One);
Zero And(One, Zero);
Zero And(Zero, One);
Zero And(Zero, Zero);

One Or(One, One);
One Or(One, Zero);
One Or(Zero, One);
Zero Or(Zero, Zero);

template <typename X1, typename X2, typename X3, typename X4>
auto Circuit(X1 x1, X2 x2, X3 x3, X4 x4) {
  auto or1 = Or(x1, x2);
  auto and1 = And(x2, x3);
  auto and2 = And(or1, and1);
  auto or2 = Or(or1, and2);
  auto or3 = Or(and1, x4);
  auto and3 = And(or2, or3);
  return and3;
}

template <typename T>
void Success(T*) { }

int main(int argc, char **argv) {
    // Success(Circuit(zero,zero,zero,zero));
    // Success(Circuit(one,zero,zero,zero));
    Success(Circuit(one,zero,zero,one));
    static_assert(false, "Halt");
}