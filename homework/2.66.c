#include <stdio.h>
#include <assert.h>

int leftmost_one(unsigned x);

int leftmost_one(unsigned x)
{
    x |= x>>16;
    x |= x>>8;
    x |= x>>4;
    x |= x>>2;
    x |= x>>1;

    return (x>>1)+(x&1);
}

int main()
{
    assert(leftmost_one(0xff00) == 0x8000);
    assert(leftmost_one(0x6600) == 0x4000);
    assert(leftmost_one(0x0) == 0x0);
    assert(leftmost_one(0x80000000) == 0x80000000);
    return 0;
}
