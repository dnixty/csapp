/*
long loop(long x, int n)
x in %rdi, n in %esi
loop:
  movl  %esi, %ecx  // ecx = n
  movl  $1, %edx    // edx = 1
  movl  $0, %eax    // eax = 0
  jmp   .L2
.L3:
  movq  %rdi, %r8   // r8 = x
  andq  %rdx, %r8   // r8 = x & rdx
  orq   %r8, %rax   // rax = (x & rdx) | 0
  salq  %cl, %rdx   // rdx = rdx << n
.L2:
  testq %rdx, %rdx  // rdx & rdx
  jne   .L3         // J .L3 if rdx != 0
  rep; ret
*/

long loop(long x, long n)
{
    long result = 0;
    long mask;
    for (mask = 1; mask != 0; mask <<= n) {
        result |= x & mask;
    }
    return result;
}

/*
A.
x in %rdi, n in %esi, result in %rax, mask in %rdx,

B.
initial values:
result: 0
mask:   1

C.
mask != 0

D.
mask <<= n

E.
result |= x & mask
*/
