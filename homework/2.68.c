#include <stdio.h>
#include <limits.h>
#include <assert.h>

int lower_one_mask(int n);

int lower_one_mask(int n)
{
    return (((0x1<<(n-1))-1)<<1)+1;
}

int main()
{
    assert(lower_one_mask(6) == 0x3f);
    assert(lower_one_mask(17) == 0x1ffff);
    assert(lower_one_mask(1) == 0x1);
    assert(lower_one_mask(32) == -1);
    return 0;
}
