#include <stdio.h>
#include <assert.h>

int fits_bits(int x, int n);

int fits_bits(int x, int n)
{
    int sign = (x>>31)&0x1;
    int offset = (x>>(n-1));

    return !(offset+sign);
}

int main()
{
    assert(!fits_bits(0xFF, 8));
    assert(!fits_bits(~0xFF, 8));
    assert(fits_bits(0b0010, 3));
    assert(!fits_bits(0b1010, 3));
    assert(!fits_bits(0b0110, 3));
    assert(fits_bits(~0b11, 3));
    assert(!fits_bits(~0b01000011, 3));
    assert(!fits_bits(~0b111, 3));
    assert(fits_bits(12, 5));
    assert(!fits_bits(12, 4));
    assert(fits_bits(0, 1));
    assert(fits_bits(-8, 5));
    assert(fits_bits(-8, 4));
    assert(!fits_bits(-8, 3));
    assert(fits_bits(-1, 1));
    assert(!fits_bits(1, 1));
    assert(fits_bits(1, 2));
    assert(fits_bits(-2, 2));
    assert(!fits_bits(8, 3));
    assert(fits_bits(8, 5));
    assert(fits_bits(-1, 10));

    return 0;
}
