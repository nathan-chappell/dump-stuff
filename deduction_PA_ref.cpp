#include <iostream>
#include <type_traits>

using namespace std;

#define PRINT_T { cout << typeid(T).name() << endl; }

template <typename T> void t_rref_plain(T&&) PRINT_T        // P = T&&
template <typename T> void t_rref_const(const T&&) PRINT_T  // P = const T&&

void int_f(int) {}

template <typename T> void f(T&& t) { cout << typeid(T).name() << endl; t = 3; }

int main(int argc, char **argv) {
  int x = 0;
  int &rx = x;
  const int &crx = 0;

  // f(crx); // d
  {
    auto fp = t_rref_plain<const int>;
    cout << typeid(decltype(fp)).name() << endl;
  }

  {
    auto fp = t_rref_plain<int>;
    cout << typeid(decltype(fp)).name() << endl;
  }

  // the name of a variable is an lvalue
  // literals are prvalues
  // a function call who returns an rvalue-reference is an xvalue

  cout << "t_rref_plain" << endl;

  t_rref_plain(x);    // A = (lvalue)  int, P = T,  T = int
  t_rref_plain(0);    // A = (prvalue) int, P = T,  T = int
  t_rref_plain(crx);  // A = (lvalue)  const int&, P = T, T = const int
  t_rref_plain(move(crx));  // A = (xvalue) const int&, P = T, T = const int

  cout << "t_rref_const" << endl;

  // t_rref_const(x);     // A = (lvalue) int,        P = const T&&, deduction fails
  t_rref_const(0);        // A = (rvalue) int,        P = const T&&, T = int
  // t_rref_const(crx);   // A = (lvalue) const int,  P = const T&&, T = int
  // compilation fails when trying to bind lvalue to rvalue reference
  t_rref_const(move(crx)); // A = (xvalue) const int&&, P = const T&&, T = int
  // compilation succeeds when trying to bind xvalue to rvalue reference

  t_rref_plain(int_f);  // A = void(int), P = void(*)(T), T = int
  t_rref_const(int_f);  // A = void(int), P = void(*)(T), T = int
}