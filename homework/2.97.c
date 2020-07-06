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

float_bits float_i2f(int i)
{
    int bits = 0;
    int bias = 127;
    int x;
    unsigned f;
    unsigned exp;
    unsigned sig = i>>31;
    (sig && (i = ~i+1));

    if (i == 0)
        return 0;

    if (i == INT_MIN) {
        sig = 1;
        exp = bias + 31;
        f = 0;
        return sig << 31 | exp << 23 | f;
    }

    x = i;
    while (x>0) {
        x >>= 1;
        bits++;
    }

    bits -= 1;

    f = ((unsigned) i << (32-bits))>>9;

    exp = bits + bias;
    return sig<<31 | exp<<23 | f;
}


int main()
{
    assert(float_i2f(7) == f2fb(7.0));
    assert(float_i2f(3) == f2fb(3.0));
    assert(float_i2f(-3) == f2fb(-3.0));
    assert(float_i2f(-2147483648) == f2fb(-2147483648.0));
    return 0;
}
