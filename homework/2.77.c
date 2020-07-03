#include <stdio.h>
#include <assert.h>

int A(int x);
int B(int x);
int C(int x);
int D(int x);

int A(int x)
{
    return (x<<4)+x;
}

int B(int x)
{
    return x-(x<<3);
}

int C(int x)
{
    return (x<<6)-(x<<2);
}

int D(int x)
{
    return (x<<4)-(x<<7);
}

int main()
{
    int x1 = 0x12345678;
    int x2 = 0x87654321;

    assert(A(x1) == x1*17);
    assert(A(x2) == x2*17);
    assert(B(x1) == x1*-7);
    assert(B(x2) == x2*-7);
    assert(C(x1) == x1*60);
    assert(C(x2) == x2*60);
    assert(D(x1) == x1*-112);
    assert(D(x2) == x2*-112);

    return 0;
}
