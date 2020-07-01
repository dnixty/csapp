#include <stdio.h>
#include <assert.h>

int main()
{
    size_t x = 0x89abcdef;
    size_t y = 0x76543210;

    size_t r = (y & ~(0xff)) | (x & 0xff);
    assert(r == 0x765432ef);

    return 0;
}
