#include <iostream>

using namespace std;

struct IntMonoid {
  int value;
  IntMonoid(int value = int()) : value{value} {}
  IntMonoid operator+(IntMonoid m) {
    return IntMonoid{value + m.value};
  }
};

struct StringMonoid {
  string value;
  StringMonoid(string value = string()) : value{value} {}
  StringMonoid operator+(StringMonoid m) {
    return StringMonoid{value + m.value};
  }
};

IntMonoid Sum(IntMonoid (&array)[3]) {
  auto result = IntMonoid();
  for (auto value : array) {
    result = result + value;
  }
  return result;
}

StringMonoid Sum(StringMonoid (&array)[3]) {
  auto result = StringMonoid();
  for (auto value : array) {
    result = result + value;
  }
  return result;
}

int main(int argc, char **argv) {
  IntMonoid mi[] = {1, 2, 3};
  cout << Sum(mi).value << endl;

  string foo = "foo", bar = "bar", baz = "baz";

  StringMonoid ms[] = {foo, bar, baz};
  cout << Sum(ms).value << endl;
}