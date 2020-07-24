typedef struct {
    long a[2];
    long *p;
} strA;

typedef struct {
    long u[2];
    long q;
} strB;

strB process(str A s) {
    strB r;
    r.u[0] = s.a[1];
    r.u[1] = s.a[0];
    r.q = *s.p;
    return r;
}

long eval (long x, long y, long z) {
    strA s;
    s.a[0] = x;
    s.a[1] = y;
    s.p = &z;
    strB r = process(s);
    return r.u[0] + r.u[1] + r.q;
}

/*
strB process(strA s)
process:
  movq %rdi, %rax     // %rax = %rsp + 64
  movq 24(%rsp), %rdx // %rdx = &z
  movq (%rdx), %rdx   // rdx = z
  movq 16(%rsp), %rcx // rcx = y
  movq %rcx, (%rdi)   // %rsp + 64 = y
  movq 8(%rsp), %rcx  // rcx = x
  movq %rcx, 8(%rdi)  // %rsp + 72 = x
  movq %rdx, 16(%rdi) // %rsp + 80 = z
  ret

long eval(long x, long y, long z)
x in %rdi, y in %rsi, z in %rdx
eval:
  subq $104, %rsp
  movq %rdx, 24(%rsp)
  leaq 24(%rsp), %rax
  movq %rdi, (%rsp)
  movq %rsi, 8(%rsp)
  movq %rax, 16(%rsp)
  leaq 64(%rsp), %rdi
  call process
  movq 72(%rsp), %rax
  addq 64(%rsp), %rax
  addq 80(%rsp), %rax
  addq $104, %rsp
  ret

A.
| ret addr |
|----------| 104
|          |
|          |
|          |
|          |
|          | rdi
|          |
|          |
|          |
|          |
| z        |
| &z       |
| y        |
| x        |
|----------| rsp

B.
%rsp + 64

C. process uses stack pointer with an offset.

D. eval pass %rsp+64 and process is using this address as a base. At the
end it returns this address.

E.
| ret addr |
|----------| 104
|          |
|          |
| z        |
| x        |
| y        | rdi
|          |
|          |
|          |
|          |
| z        |
| &z       |
| y        |
| x        |
|----------| rsp


F.  The caller is computing the space needed for both the struct it's
using and the struct that is returned from the function beeing
called. Callee knows that the struct passed in the argument begins at
the %rsp. It also knows that he can find the value of it's own base in
%rdi register.
*/
