#include <stdio.h>
#include <limits.h>
#include <assert.h>

int saturating_add(int x, int y);

int saturating_add(int x, int y)
{
    int x_msk = (x >> 31);
    int y_msk = (x >> 31);

    int sum = x+y;
    int sum_msk = (sum >> 31);

    int posof_msk = ~x_msk & ~y_msk & sum_msk;
    int negof_msk = x_msk & y_msk & ~sum_msk;

    return (posof_msk & INT_MAX) | (negof_msk & INT_MIN) |
        (~posof_msk & ~negof_msk & sum);
}

int main()
{
    assert(saturating_add(INT_MAX, 1) == INT_MAX);
    assert(saturating_add(INT_MIN, -1) == INT_MIN);
    assert(saturating_add(4, 5) == 9);
    assert(saturating_add(-4, -5) == -9);
    return 0;
}
