#include <stdio.h>
#include <assert.h>

typedef unsigned float_bits;

float_bits f2fb(float f)
{
    return *(float_bits *)&f;
}

float_bits float_twice(float_bits uf) {
    unsigned s = uf>>31;
    unsigned e = ((uf<<1)>>24);
    unsigned f = uf&0x7fffff;

    if (e == 0xff)
        return uf;
    else if (e == 0xff-1) {
        e = 0xff;
        f = 0;
    } else if (e)
        e += 1;
    else
        f <<= 1;

    return (s<<31)|(e<<23)|f;
}

int main()
{
    assert(float_twice(f2fb(1.0)) == f2fb(2.0));
    assert(float_twice(f2fb(1.5)) == f2fb(3.0));
    return 0;
}
