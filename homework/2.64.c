#include <stdio.h>
#include <assert.h>

int any_odd_one(unsigned x);

int any_odd_one(unsigned x)
{
    int mask = 0xAAAAAAAA;
    return !!(x & mask);
}

int main()
{
    int x1 = 0xa50; // 101001010000
    int x2 = 0x150; // 000101010000

    assert(any_odd_one(x1));
    assert(!any_odd_one(x2));

    return 0;
}
