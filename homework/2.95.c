#include <stdio.h>
#include <assert.h>

typedef unsigned float_bits;

float_bits f2fb(float f)
{
    return *(float_bits *)&f;
}

float_bits float_half(float_bits uf) {
    unsigned s = uf>>31;
    unsigned r = uf & 0x7fffffff;
    unsigned e = ((uf<<1)>>24);

    unsigned f = uf&0x7fffff;

    int rte = (f & 0x3) == 0x3;

    if (e == 0xff)
        return uf;
    else if (e == 1) {
        r >>= 1;
        r += rte;
        e = r >> 23 & 0xff;
        f = r & 0x7fffff;
    } else if (e)
        e -= 1;
    else {
        f >>= 1;
        f += rte;
    }

    return (s<<31)|(e<<23)|f;
}

int main()
{
    assert(float_half(f2fb(2.0)) == f2fb(1.0));
    assert(float_half(f2fb(3.0)) == f2fb(1.5));
    return 0;
}
