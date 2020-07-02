#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void copy_int(int val, void *buf, int maxbytes);

/* This implementation is buggy. sizeof returns an size_t value which is a kind of unsigned value. maxbytes-sizeof(val) gets size_t value, which is always non-negative */
void copy_int(int val, void *buf, int maxbytes)
{
    if (maxbytes >= (int) sizeof(val))
        memcpy(buf, (void *) &val, sizeof(val));
}

int main()
{
    int maxbytes = sizeof(int) * 10;
    void *buf = malloc(maxbytes);
    int val;

    val = 0x12345678;
    copy_int(val, buf, maxbytes);
    assert(*(int*)buf == val);

    val = 0xaabbccdd;
    copy_int(val, buf, 0);
    assert(*(int*)buf != val);

    free(buf);
    return 0;
}
