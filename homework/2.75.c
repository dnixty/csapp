#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

unsigned unsigned_high_prod(unsigned x, unsigned y);
int signed_high_prod(int x, int y);

unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    int sig_x = x >> 31;
    int sig_y = y >> 31;
    int signed_prod = signed_high_prod(x, y);
    return signed_prod + x * sig_y + y * sig_x;
}

int signed_high_prod(int x, int y)
{
    int64_t mul = (int64_t) x * y;
    return mul >> 32;
}

unsigned another_unsigned_high_prod(unsigned x, unsigned y) {
    uint64_t mul = (uint64_t) x * y;
    return mul >> 32;
}


int main()
{
    unsigned x = 0x12345678;
    unsigned y = 0xffffffff;

    assert(another_unsigned_high_prod(x, y) == unsigned_high_prod(x, y));
    return 0;
}
