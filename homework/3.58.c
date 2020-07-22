#include <stdio.h>
#include <assert.h>

long decode2(long x, long y, long z);

/*
decode2:
  subq  %rdx, %rsi // rsi = y - z
  imulq %rsi, %rdi // rdi = x * (y - z)
  movq  %rsi, %rax // val = y - z
  salq  $63, %rax  // val << 63;
  sarq  $63, %rax  // val >> 63;
  xorq  %rdi, %rax // val = x * (y - z) ^ val
  ret
*/

long decode2(long x, long y, long z)
{
    long val = x*(y - z);

    if (x < z)
        return val;
    else
        return ~val;
}

int main()
{
    return 0;
}
