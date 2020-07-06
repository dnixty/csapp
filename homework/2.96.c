#include <stdio.h>
#include <assert.h>

typedef unsigned float_bits;

float_bits f2fb(float f)
{
    return *(float_bits *)&f;
}

int float_f2i(float_bits f)
{
    unsigned sig = f >> 31;
    unsigned exp = f >> 23 & 0xff;
    unsigned frac = f & 0x7fffff;
    int bias = 127;
    int r;

    if (exp == 0xff) {
        return 0x80000000;
    } else if (exp == 0) {
        return 0;
    } else {
        r = 1<<(exp-bias) | frac>>(23-exp+bias);
        (sig && (r = ~r+1));
        return r;
    }
}


int main()
{
    assert(float_f2i(f2fb(2.0)) == 2);
    assert(float_f2i(f2fb(3.0)) == 3);
    assert(float_f2i(f2fb(-3.5)) == -3);
    assert(float_f2i(0x7f800001) == 0x80000000);
    assert(float_f2i(0x001fe123) == 0);
    return 0;
}
