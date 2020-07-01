#include <stdio.h>
#include <assert.h>

int int_shifts_are_arithmetic();

int int_shifts_are_arithmetic()
{
    return !((~0)>>1 ^ ~(0));
}

int main()
{
    assert(int_shifts_are_arithmetic());

    return 0;
}
