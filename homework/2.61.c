#include <stdio.h>
#include <assert.h>

int A(int x);
int B(int x);
int C(int x);
int D(int x);

int A(int x)
{
    return !!x;
}

int B(int x)
{
    return !x;
}

int C(int x)
{
    return (x & 0xf);
}

int D(int x)
{
    return (x >> ((sizeof(int)-1) << 3)) & 0xf;
}

int main()
{
    int x1 = ~0;
    int x2 = 0;
    int x3 = 0x1fc0a;
    int x4 = 0x1fca0;

    assert(A(x1));
    assert(!A(x2));
    assert(!B(x1));
    assert(B(x2));
    assert(C(x3));
    assert(!C(x4));
    assert(D(x1));
    assert(!D(x4));

    return 0;
}
