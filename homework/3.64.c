#define R 7
#define S 13
#define T 5

long A[R][S][T];

long store_ele(long i, long j, long k, long *dest)
{
    *dest = A[i][j][k];
    return sizeof(A);
}

/*
long store_ele(long i, long j, long k, long *dest)
i in %rdi, j in %rsi, k in %rdx, dest in %rcx
store_ele:
  leaq  (%rsi, %rsi, 2), %rax  // rax = 3*j
  leaq  (%rsi, %rax, 4), %rax  // rax = 13*j
  movq  %rdi, %rsi             // rsi = i
  salq  $6, %rsi               // rsi = i*64
  addq  %rsi, %rdi             // rdi = i*65
  addq  %rax, %rdi             // rdi = i*65+13*j
  addq  %rdi, %rdx             // rdx = i*65+13*j+k
  movq  A(,%rdx,8), %rax       // rax = 8*(65*i + 13*j + k) + x_A
  movq  %rax, (%rcx)           // *dest = 8*(65*i + 13*j + k) + x_A
  movl  $3640, %eax            // eax = 3640
  ret

&A[i][j][k] = x_D + L(T(S*i + j)+k)

rax = 8*(5*(13*i + j) + k) + A
T = 5
S = 13
R = 3640/5/13/8 = 7
*/
