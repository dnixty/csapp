#include <stdio.h>
#include <limits.h>
#include <assert.h>

int tsub_ok(int x, int y);

int tsub_ok(int x, int y)
{
    int diff = x-y;
    int ovflw = (x >> 31) & ~(y >> 31) & ~(diff >> 31);
    return !(ovflw || y==INT_MIN);
}

int main()
{
    assert(tsub_ok(INT_MIN+1, 1));
    assert(!tsub_ok(INT_MIN, INT_MIN));
    assert(!tsub_ok(INT_MIN, 1));
    assert(tsub_ok(-1, -2));
    return 0;
}
