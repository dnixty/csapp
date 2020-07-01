#include <stdio.h>

typedef unsigned char* byte_pointer;

int is_little_endian();

int is_little_endian()
{
    int v = 0x0f;
    return *((byte_pointer) &v) == 0xf;
}

int main()
{
    printf("%d\n", is_little_endian());
}
