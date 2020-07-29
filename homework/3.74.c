#include <stdio.h>
#include <assert.h>

typedef enum {NEG, ZERO, POS, OTHER} range_t;

range_t find_range(float x)
{
    __asm__(
            "vxorps %xmm1, %xmm1, %xmm1\n\t"
            "vucomiss %xmm1, %xmm0\n\t"
            "movl $1, %eax\n\t"
            "movl $2, %ebp\n\t"
            "cmova %ebp, %eax\n\t"
            "movl $0, %ebp\n\t"
            "cmovb %ebp, %eax\n\t"
            "movl $3, %ebp\n\t"
            "cmovp %ebp, %eax\n\t"
           );
}

int main()
{
    range_t n = NEG, z = ZERO, p = POS, o = OTHER;
    assert(o == find_range(0.0/0.0));
    assert(n == find_range(-2.3));
    assert(z == find_range(0.0));
    assert(p == find_range(3.33));
    return 0;
}
