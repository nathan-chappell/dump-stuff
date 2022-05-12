#include <iostream>
#include <type_traits>

using namespace std;

struct Foo {
  Foo() = delete;
  const int &cri;
};

int main(int argc, char **argv) {
  int int_x = 0;
  const int cint_x = 0;
  int &&rref_int_x = (int&&)int_x;

  decltype(declval<Foo>().cri) cri = int_x;
  // cri = 3;

  #define TEST_MOD(E, BindTo, val) \
  { \
    cout << "E: " << #E << " " << E << endl; \
    cout << BindTo << endl; \
    decltype(E) r1 = BindTo; \
    r1 = val; \
    cout << E << endl; \
  }

  TEST_MOD(int_x, int_x, 10);
  TEST_MOD((int_x), int_x, 20);
  // TEST_MOD(cint_x, 30);
  // TEST_MOD((cint_x), 40);
  // TEST_MOD(rref_int_x, 50);
  TEST_MOD(rref_int_x, 0, 50);
  // TEST_MOD((rref_int_x), 0, 60);
  // TEST_MOD(0, 70);
}