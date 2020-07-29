#define CNT 7

typedef struct {
    long idx;
    long x[4];
} a_struct;

typedef struct {
    int first;       // offset 0
    a_struct a[CNT]; // offset 8
    int last;        // offset: 288
} b_struct;

void test(long i, b_struct *bp)
{
    int n = bp->first + bp->last;
    a_struct *ap = &bp->a[i];
    ap->x[ap->idx] = n;
}

// sizeof(a_struct) = 40

/*
void test(long i, b_struct *bp)
i in %rdi, bp in %rsi

test:
  mov    0x120(%rsi), %ecx       // ecx = bp->last
  add    (%rsi), %ecx            // ecx = bp->first + bp->last
  lea    (%rdi,%rdi,4), %rax     // rax = 5*i
  lea    (%rsi,%rax,8), %rax     // rax = bp + 40*i
  mov    0x8(%rax), %rdx         // rdx = *(bp + 40*i + 8) idx
  movslq %ecx, %rcx              // rcx = (long) n
  mov    %rcx, 0x10(%rax,%rdx,8) // (bp + 40*i + 16) + 8*idx = n
  retq
*/
