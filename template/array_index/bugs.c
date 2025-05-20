#include <assert.h>
#include <klee/klee.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 0x100

char get_value_at_index(char *ary, int ary_size, int i);
char get_value_at_index_oracle(char *ary, int ary_size, int i);

int main() {
  char buf[MAX_SIZE];
  int size;
  int i;

  klee_make_symbolic(&i, sizeof(i), "i");

  klee_make_symbolic(&size, sizeof(size), "size");
  klee_assume(size <= MAX_SIZE);
  klee_assume(size > 0);
  klee_make_symbolic(buf, MAX_SIZE, "buffer");
  klee_make_symbolic(&i, sizeof(i), "i");

  klee_assert(get_value_at_index(buf, sizeof(buf), i) ==
              get_value_at_index_oracle(buf, sizeof(buf), i));
  return 0;
}
