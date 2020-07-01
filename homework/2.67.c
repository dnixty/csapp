#include <stdio.h>
#include <assert.h>

//int bad_int_size_is_32();
int int_size_is_32_B();
int int_size_is_32_C();

/* A. bad_int_size_is_32 shifts the value more than 31 bits which with
 * the C standard is an undefined operation */
// int bad_int_size_is_32()
// {
//     int set_msb = 1 << 31;
//     int beyond_msb = 1 << 32;
//
//     return set_msb && !beyond_msb;
// }

int int_size_is_32_B()
{
    int set_msb = 1 << 31;
    int beyond_msb = set_msb << 1;

    return set_msb && !beyond_msb;
}

int int_size_is_32_C()
{
    int set_msb = 1 << 15 << 15 << 1;
    int beyond_msb = set_msb << 1;

    return set_msb && !beyond_msb;
}

int main()
{
    assert(int_size_is_32_B());
    assert(int_size_is_32_C());
    return 0;
}
