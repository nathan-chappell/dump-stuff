struct A {
    int a;
    virtual void f_virtual() {}
    A() { a = 17; }
    virtual ~A() { a = -1; }
};

struct B : virtual A {
    int b;
    B() { b = 18; }
    virtual ~B() { b = -2; }
};

struct C : virtual A {
    int c;
    C() { c = 19; }
    ~C() { c = -3; }
};

struct D : B, C {
    int d;
    virtual void f_virtual() {}
    D() { d = 20; }
    ~D() { d = -4; }
};

template<typename T, typename U>
bool ref_equality(const T &t, const U &u) {
    return &t == &u;
}

extern "C" void *_ZTT1D;

void Demo() {
    D *d_p = new D();
    A *d_as_a = d_p;
    (*d_as_a).f_virtual();

    C *d_as_c = d_p;
    d_as_c->f_virtual();

    delete d_as_a;
}


int main(int argc, char **argv) {
    Demo();
}