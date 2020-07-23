long switch_prob(long x, long n) {
    long result = x;
    switch(n) {
    case 60:
    case 62:
        result = 8*x;
        break;
    case 63:
        result = x >> 3;
        break;
    case 64:
        x = (x<<4)-x;
    case 65:
        x = x * x;
    default:
        result = x + 75;
        break;
    }
    return result;
}

/*
x/6gx 0x4006f8
0x4006f8: 0x00000000004005a1 0x00000000004005c3
0x400708: 0x00000000004005a1 0x00000000004005aa
0x400718: 0x00000000004005b2 0x00000000004005bf
*/


/*
long switch_prob(long x, long n)
x in %rdi, n in %rsi

switch_prob:
  sub   $0x3c, %rsi // n -= 60
  cmp   $0x5, %rsi  // n - 5
  ja    .L1
  jmpq  *0x4006f8(,%rsi,8)
.L0
  lea   0x0(,%rdi,8), %rax // result = 8x
  00 (noop?)
  retq
.L3
  mov   %rdi, %rax // result = x
  sar   $0x3, %rax // result = x >> 3
  retq
.L4:
  mov   %rdi, %rax // result = x
  shl   $0x4, %rax // result = x << 4
  sub   %rdi, %rax // result = (x<<4)-x
  mov   %rax, %rdi // rdi = result
.L5:
  imul  %rdi, %rdi // rdi = result * result
.L1:
  lea   0x4b(%rdi), %rax // result = x + 75
  retq
