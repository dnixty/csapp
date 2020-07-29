#include <alloca.h>

long aframe(long n, long idx, long *q) {
    long i;
    long **p = alloca(n * sizeof(long *));
    p[0] = &i;
    for (i = 1; i < n; i++)
        p[i] = q;
    return *p[idx];
}

/*
long aframe(long n, long idx, long *q)
n in %rdi, idx in %rsi, q in %rdx
aframe:
  pushq  %rbp              // save rbp; rsp -= 8
  movq   %rsp, %rbp        // rbp = $rsp (frame pointer)
  subq   $16, %rsp         // Allocate space for i (%rsp = s_1)
  leaq   30(,%rdi,8), %rax // rax = 8*n + 30
  andq   $-16, %rax        // Round rax down to the nearest multiple of 16
  subq   %rax, %rsp        // Allocate space for array p (%rsp = s_2)
  leaq   15(%rsp), %r8     // r8 = s2 + 15
  andq   $-16, %r8         // Set %r8 to %p[0] rounded to multiple of 8
  .
  .
  .

A.
s_2 = s_1 - (8*n + 30)&-16

if n is odd then s_2 = s_1 - (8*n + 24)
if n is even then s_2 = s_1 - (8*n + 16)

B.
p = (s_2 + 15)&-16
the least multiple of 16 that is within s2

C.
e_1 can't be zero as that would mean p == s_2 which is a contradiction

When n is even, e_1 + e_2 = 16. If e_2 == 15 then e_1 will be least that
can reach, which is 1.
s1 == n that n%16 == 1

When n is odd, e_1 + e_2 = 24. If p == s2, e2 == 0, so e1 will be the
greatest value 24 that can reach.
s1 == n that n%16 == 0

D. p i aligned by 16. s_2 is the least multiple of 16 that preserve 8*n
size space
*/
