union ele {
    struct {
        long *p;
        long y;
    } e1;
    struct {
        long x;
        union e1e *next;
    } e2;
};

void proc (union ele *up) {
    up->e2.x = *(*(up->e2.next)->p) - *(up->e2.next).e1.y;
}

/*
A.
e1.p    0
e1.y    8
e2.x    0
e2.next 8

B.
16

C.
void proc (union ele *up)
up in %rdi
proc:
  movq    8(%rdi), %rax  // rax = *(up + 8) = up->e2.next
  movq    (%rax), %rdx   // rdx = *(*(up + 8)) = *(up->e2.next)
  movq    (%rdx), %rdx   // rdx = *(*(up->e2.next)) = *(*(up->e2.next).e1.p)
  subq    8(%rax), %rdx  // rdx = *(*(up->e2.next)->p) - *(*(up->e2.next) + 8) = *(*(up->e2.next)->p) - *(up->e2.next).e1.y;
  movq    %rdx, (%rdi)   // up->x = rdx
  ret
*/
