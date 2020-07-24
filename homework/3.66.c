#define NR(n) 3*n
#define NC(n) 4*n+1

long sum_col(long n, long A[NR(n)][NC(n)] long j) {
    long i;
    long result = 0;
    for (i = 0; i < NR(n); i++)
        result += A[i][j];
    return result;
}

/*
long sum_col(long n, long A[NR(n)][NC(n)], long j)
n in %rdi, A in %rsi, j in %rdx

sum_col:
  leaq    1(,%rdi,4), %r8     // %r8 = 4*n + 1
  leaq    (%rdi,%rdi,2), %rax // rax = 3*n
  movq    %rax, %rdi          // rdi = 3*n
  testq   %rax, %rax          // 3*n & 3*n
  jle     .L4                 // if (3*n <= 0) J .L4
  salq    $3, %r8             // %r8 = 8*(4*n + 1)
  leaq    (%rsi,%rdx,8), %rcx // rcx = A + 8*j
  movl    $0, %eax            // eax = 0 (result = 0)
  movl    $0, %edx            // edx = 0 (i = 0)
.L3:
  addq    (%rcx), %rax        // rax = *(A + 8*j)
  addq    $1, %rdx            // rdx = i++
  addq    %r8, %rcx           // rcx = A + 8*j + 8*(4*n + 1)
                              // rcx = A + 8(j + 4*n + 1)
  cmpq    %rdi, %rdx          // i - 3*n
  jne     .L3
  rep; ret
.L4:
  movl    $0, %eax
  ret
