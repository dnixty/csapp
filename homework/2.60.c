#include <stdio.h>
#include <assert.h>

unsigned replace_byte (unsigned x, int i, unsigned char b);

unsigned replace_byte (unsigned x, int i, unsigned char b)
{
    if (i < 0) {
        printf("error: i is negative\n");
        return x;
    }
    if (i > sizeof(unsigned)-1) {
        printf("error: too big i\n");
        return x;
    }

    unsigned mask = ((unsigned) 0xff) << (i << 3);
    unsigned pos_byte = ((unsigned) b) << (i << 3);

    return (x & ~mask) | pos_byte;
}

int main()
{
    unsigned x = 0x12345678;
    unsigned char b = 0xab;

    assert(replace_byte(x, 2, b) == 0x12ab5678);
    assert(replace_byte(x, 0, b) == 0x123456ab);

    return 0;
}
