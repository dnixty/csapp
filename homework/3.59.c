#include <stdio.h>

typedef __int128 int128_t;

void store_prod(int128_t *dest, int64_t x, int64_t y) {
    *dest = x * (int128_t) y;
}

/*
ux = x + x_63 * 2^64
uy = y + x_63 * 2^64

ux * uy = (x + x_63 * 2^64) * (y + y_63 * 2^64)
ux * uy = (x * y + (x_63*y + y_63*x)*2^64 + x_63*y_63*2^128
We can drop 2^128 element as it overflows.

x*y = ux*uy - (x_63*y + y_63*x)*2^64

store_prod:
  movq    %rdx, %rax    // %rax = y
  cqto                  // (int128_t) y, %rdx = (-1)y_63
  movq    %rsi, %rcx    // %rcx = x
  sarq    $63, %rcx     // x >>= 63 (sign)
  imulq   %rax, %rcx    // %rcx = y*(x<0 ? -1 : 0)
  imulq   %rsi, %rdx    // %rdx = x* -y_63
  addq    %rdx, %rcx    // %rcx = x * -y_63 + y * -x_63
  mulq    %rsi          // %rdx:%rax <= ux * uy
  addq    %rcx, %rdx    // %rdx = ux * uy(high 64 bits) -
                        //        (x_{63}y + y_{63}x)2^{64}
  movq    %rax, (%rdi)  // set lower 64 bits
  movq    %rdx, 8(%rdi) // set higher 64 bits
  ret
*/
