#include <stdio.h>
#include <assert.h>

unsigned srl(unsigned x, int k)
{
    unsigned xsra = (int) x >> k;
    int w = sizeof(int) << 3;

    int mask = ~0 << (w-k);

    return xsra & ~mask;
}

int sra(int x, int k)
{
    int xsrl = (unsigned) x >> k;
    int w = sizeof(int) << 3;

    int isneg = !!(x & (~0 << (w - 1)));
    int mask = ~((isneg << (w-k-1))-1);

    return xsrl | mask;
}

int main()
{
    unsigned x1 = ~0;
    unsigned x2 = 0x80000000;

    assert(srl(x1, 4) == 0x0fffffff);
    assert(sra(x1, 4) == -1);
    assert(sra(x2, 6) == 0xfe000000);
    return 0;
}
