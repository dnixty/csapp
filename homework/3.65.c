void transpose(long A[M][M]) {
    long i, j;
    for (i = 0; i < M; i++)
        for (j = 0; j < i; j++) {
            long t = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = t;
        }
}

/*
.L6:
  movq  (%rdx), %rcx
  movq  (%rax), %rsi
  movq  %rsi, (%rdx)
  movq  %rcx, (%rax)
  addq  $8, %rdx
  addq  $120, %rax
  cmpq  %rdi, %rax
  jne   .L6

A.
&A[i][j] in %rdx
B.
&A[j][i] in %rax
C.
M = 120 / 8 = 15
*/
