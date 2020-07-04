#include <stdio.h>
#include <assert.h>

int float_le(float x, float y);
unsigned f2u(float x);

int float_le(float x, float y)
{
    unsigned ux = f2u(x);
    unsigned uy = f2u(y);

    unsigned sx = ux >> 31;
    unsigned sy = uy >> 31;

    return (ux << 1 == 0 && uy << 1 == 0) ||
        (sx && !sy) ||
        (!sx && !sy && ux <= uy) ||
        (sx && sy && ux >= uy);
}

unsigned f2u(float x)
{
    return *(unsigned *)&x;
}

int main()
{
    assert(float_le(2.5, 3.5));
    assert(float_le(0.0, 0.0));
    assert(float_le(0.003, 0.03));
    assert(!float_le(0.4321, 0.4320));
    return 0;
}
