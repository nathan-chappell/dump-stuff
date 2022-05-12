#include <iostream>
#include <type_traits>

using namespace std;

#define PRINT_T { cout << typeid(T).name() << endl; }

template <typename T> void t_plain(T) PRINT_T           // P = T
template <typename T> void t_lref_plain(T&) PRINT_T     // P = T
template <typename T> void t_pointer(T *) PRINT_T       // P = T*
template <typename T> void t_function(void(T)) PRINT_T  // P = void(*)(T)

void int_f(int) {}

int main(int argc, char **argv) {
  int x = 0;
  int *x_p = &x;

  t_plain(x);         // A = int,   P = T,  T = int
  t_plain(x_p);       // A = int*,  P = T,  T = int*
  t_lref_plain(x);   // A = int,   P = T,  T = int
  t_lref_plain(x_p); // A = int*,  P = T,  T = int*
  // t_pointer(x);    // A = int,   P = T*, deduction fails
  t_pointer(x_p);     // A = int*,  P = T*, T = int

  t_function(int_f);  // A = void(*)(int), P = void(*)(T), T = int
}