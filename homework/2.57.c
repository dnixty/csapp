#include <stdio.h>

typedef unsigned char* byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    printf(" %.2x", start[i]);
  }
  printf("\n");
}

void show_short(short x) {
    show_bytes((byte_pointer) &x, sizeof(short));
}

void show_long(long x) {
    show_bytes((byte_pointer) &x, sizeof(long));
}

void show_double(double x) {
    show_bytes ((byte_pointer) &x, sizeof(double));
}

void test_show_bytes(int val) {
    short sval = (short) val;
    long lval = (long) val;
    double dval = (double) val;

    show_short(sval);
    show_long(lval);
    show_double(dval);
}

int main(int argc, char* argv[]) {
  int test_num = 328;

  test_show_bytes(test_num);
  return 0;
}
