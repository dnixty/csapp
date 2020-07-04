#include <stdio.h>
#include <assert.h>

unsigned A(int k)
{
    return -1<<k;
}

unsigned B(int j, int k)
{
    return (-1<<j) &
        ~(-1<<(k+j));
}

int main()
{
    assert(A(4) == 0xfffffff0);
    assert(B(4, 24) == 0x0ffffff0);

    return 0;
}
