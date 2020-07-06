#include <stdio.h>
#include <limits.h>
#include <assert.h>

typedef unsigned float_bits;

float_bits f2fb(float f)
{
    return *(float_bits *)&f;
}

float fb2f(float_bits f)
{
    return *(float *)&f;
}

float_bits float_negate(float_bits f)
{
    unsigned sig = f >> 31;
    unsigned exp = f >> 23 & 0xff;
    unsigned frac = f & 0x7fffff;

    int is_NAN = (exp == 0xff) && (frac != 0);
    if (is_NAN) {
        return f;
    }

    return ~sig << 31 | exp << 23 | frac;
}

int main()
{
    assert(float_negate(f2fb(1.0)) == f2fb(-1.0));
    assert(float_negate(f2fb(-1.5)) == f2fb(1.5));
    assert(float_negate(f2fb(0.325789)) == f2fb(-0.325789));
    return 0;
}
