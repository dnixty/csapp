#include <stdio.h>
#include <assert.h>

int odd_ones(unsigned x);

int odd_ones(unsigned x)
{
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;

    return x&1;
}


int main()
{
    assert(odd_ones(1));
    assert(!(odd_ones(0)));
    assert(!(odd_ones(-1)));
    assert((odd_ones(0xfe)));
    assert((odd_ones(0xfefe0103)));

    return 0;
}
