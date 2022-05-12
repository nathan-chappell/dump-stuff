#include <iostream>
#include <type_traits>

using namespace std;

#define PRINT_T { cout << "T: "<< typeid(T).name() << endl; }

template <typename T> void t_rref_plain(T&&) PRINT_T        // P = T
template <typename T> void t_rref_const(const T&&) PRINT_T  // P = const T&&

template <typename T> void t_modify(T&& t) { t = 3; }

void int_f(int) {}

int main(int argc, char **argv) {
  // NOTE:
  // the name of a variable is an lvalue
  // literals are prvalues
  // a function call who returns an rvalue-reference is an xvalue

  // WARNING:
  // We'll use typeid to help us name some types, but it is not perfect
  cout << "WARNING: typeid(const int&) == " << typeid(const int&).name() << endl;

  int int_x = 0;
  int &ref_x = int_x;
  const int &cref_x = 0;


  cout << "decltypes" << endl;
  #define P_DECL(E) cout << "E: " << #E << "\t" << typeid(t_rref_plain<decltype(E)>).name() << endl;
  
  P_DECL(int_x)
  P_DECL(10000)
  P_DECL(ref_x)
  P_DECL((ref_x))
  P_DECL(cref_x)
  P_DECL(int_f)
  P_DECL((int_f))

  #undef P_DECL

  // t_modify(cref_x);              // P = T, A = (lvalue) const int &, T = const int &
  // t_modify<const int &> has signature void(const int &) due to collapse
  // auto fp = t_modify<const int>; // T = const int
  // t_modify<const int> has signature void(const int &&)
  // in both cases, instantiation fails attempting to modify const int

  cout << "t_rref_plain" << endl;

  t_rref_plain(int_x);        // A = (lvalue)  int, P = T,  T = int
  t_rref_plain(0);            // A = (prvalue) int, P = T,  T = int
  t_rref_plain(cref_x);       // A = (lvalue)  const int&, P = T, T = const int
  t_rref_plain(move(cref_x)); // A = (xvalue)  const int&, P = T, T = const int

  cout << "t_rref_const" << endl;

  // t_rref_const(int_x);     // A = (lvalue) int,        P = const T&&, deduction fails
  t_rref_const(0);            // A = (rvalue) int,        P = const T&&, T = int
  
  // compilation fails when trying to bind lvalue to rvalue reference
  // t_rref_const(cref_x);    // A = (lvalue) const int,  P = const T&&, T = int

  // compilation succeeds when trying to bind xvalue to rvalue reference
  t_rref_const(move(cref_x)); // A = (xvalue) const int&&, P = const T&&, T = int

  t_rref_plain(int_f);  // A = void(int), P = T, T = void (int)
  t_rref_const(int_f);  // A = void(int), P = const T, T = void(int)

}