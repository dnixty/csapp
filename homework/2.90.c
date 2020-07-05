#include <stdio.h>
#include <assert.h>
#include <math.h>

float u2f(unsigned x)
{
    return *(float *)&x;
}

/*
bias = 2^7-1
denormalized
E = 1-2^7-1 = -2^7



*/

float fpwr2(int x)
{
    unsigned exp, frac;
    unsigned u;

    if (x < 2-pow(2,7)-23) {
        exp = 0;
        frac = 0;
    } else if (x < 2-pow(2,7)) {
        exp = 0;
        frac = 1<<(unsigned) (x - (2-pow(2,7)-23));
    } else if (x < pow(2,7)) {
        exp = pow(2,7)-1+x;
        frac = 0;
    } else {
        exp = 0xff;
        frac = 0;
    }

    u = exp << 23 | frac;
    return u2f(u);
}


int main()
{
    assert(fpwr2(0) == powf(2,0));
    assert(fpwr2(100) == powf(2,100));
    assert(fpwr2(-100) == powf(2,-100));
    assert(fpwr2(10000) == powf(2,10000));
    assert(fpwr2(-10000) == powf(2,-10000));
    return 0;
}
