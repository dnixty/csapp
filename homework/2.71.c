#include <stdio.h>
#include <assert.h>

typedef unsigned packed_t;

int xbyte(packed_t word, int bytenum);

/* This implementation of xbyte doesn't sign extend the result */
// int xbyte(packed_t word, int bytenum)
// {
//     return (word >> (bytenum << 3)) & 0xFF;
// }

int xbyte(packed_t word, int bytenum)
{
    int s = sizeof(unsigned);
    int ls = (s - bytenum - 1) << 3;
    int rs = (s - 1) << 3;

    return (int) word << ls >> rs;
}

int main()
{
    packed_t x = 0x01fff870;
    assert(xbyte(x, 0) == 0x70);
    assert(xbyte(x, 1) == -8);
    assert(xbyte(x, 2) == -1);
    assert(xbyte(x, 3) == 1);

    return 0;
}
