typdef struct {
    int x[A][B];
    long y;
} str1;

typedef struct {
    char array[B];
    int t;
    short s[A];
    long u;
} str2;

void setVal(str1 *p, str2 *q) {
    long v1 = q->t;
    long v2 = q->u;
    p->y = v1+v2;
}

/*
void setVal(str1 *p, str2 *q)
p in %rdi, q in %rsi
setVal:
  movslq  8(%rsi), %rax
  addq    32(%rsi), %rax
  movq    %rax, 184(%rdi)
  ret

Because setVal is accessing q->t with offset of 8 and int aligns the
struct by 4 bytes. We can determine that the total size of array is between 5 and 8 bytes. 4 < B <= 8

setVal is accessing u with offset of 32. Long is aligning by 8.
0 - char array[8]
8 - int t;
12 - short s[A] 2
16 - .. 4
24 - .. 4
32 - long u
40 - end

Thus 24 < A*2 + 12 <= 32
12 < A*2 <= 20
6 < A <= 10

p->y is accessing long y with
an offset of 184. Long is aligning by 8. 176 < A*B*4 <= 184

Thus 44 < A*B <= 46

4 < B <= 8
6 < A <= 10
44 < A*B <= 46

Only A = 9 and B = 5 satisfies inequalities.
*/
