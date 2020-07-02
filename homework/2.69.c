#include <stdio.h>
#include <assert.h>

unsigned rotate_left(unsigned x, int n);

unsigned rotate_left(unsigned x, int n)
{
    int w = sizeof(int) << 3;
    return (x << n) | ((x >> (w-n-1)) >> 1);
}

int main()
{
    unsigned x = 0x12345678;
    assert(rotate_left(x, 4) == 0x23456781);
    assert(rotate_left(x, 20) == 0x67812345);
    assert(rotate_left(x, 0) == x);
    assert(rotate_left(0xfffffffe, 31) == 0x7fffffff);
    return 0;
}
