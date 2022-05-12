#include <cmath>
#include <iostream>

using namespace std;

template <typename Type, double (*vtable[])(Type *)>
struct TypeWithTable : Type {
  template <int I> double Call() { return vtable[I](this); }
};

struct R2 {
  double x1;
  double x2;
};

double EuclideanX(R2 *point) { return point->x1; }
double EuclideanY(R2 *point) { return point->x2; }
double EuclideanDistance(R2 *point) { return hypot(point->x1, point->x2); }
double EuclideanAngle(R2 *point) { return atan2(point->x2, point->x1); }

double PolarX(R2 *point) { return point->x1 * cos(point->x2); }
double PolarY(R2 *point) { return point->x1 * sin(point->x2); }
double PolarDistance(R2 *point) { return point->x1; }
double PolarAngle(R2 *point) { return point->x2; }

enum { X = 0, Y = 1, DISTANCE = 2, ANGLE = 3 };

double (*EuclideanVTable[])(R2 *) = {
    EuclideanX,
    EuclideanY,
    EuclideanDistance,
    EuclideanAngle,
};

double (*PolarVTable[])(R2 *) = {
    PolarX,
    PolarY,
    PolarDistance,
    PolarAngle,
};

using EuclideanPoint = TypeWithTable<R2, EuclideanVTable>;
using PolarPoint = TypeWithTable<R2, PolarVTable>;

template <double (*vtable1[])(R2 *), double (*vtable2[])(R2 *)>
EuclideanPoint VectorSum(TypeWithTable<R2, vtable1> p1, TypeWithTable<R2, vtable2> p2) {
    auto result = EuclideanPoint();
    result.x1 = p1.template Call<X>() + p2.template Call<X>();
    result.x2 = p1.template Call<Y>() + p2.template Call<Y>();
    return result;
}

template <double (*vtable1[])(R2 *)>
ostream &operator<<(ostream &o, TypeWithTable<R2, vtable1> p) {
  return o << "{"
    << "X=" << p.template Call<X>()
    << ", Y=" << p.template Call<Y>()
    << ", DISTANCE=" << p.template Call<DISTANCE>()
    << ", ANGLE=" << p.template Call<ANGLE>()
    << "}";
}

int main(int argc, char **argv) {
  EuclideanPoint e;
  e.x1 = 3.0;
  e.x2 = 4.0;
  cout << e << endl;

  PolarPoint p;
  p.x1 = e.Call<DISTANCE>();
  p.x2 = e.Call<ANGLE>();
  cout << e << endl;

  auto q = VectorSum(e, p);
  cout << q << endl;
  return 0;
}