#include <klee/klee.h>
#include <stdlib.h>
#include <string.h>

char get_value_at_index(char *ary, int ary_size, int i);

int main() {
  int i;
  char buf[0x10];
  klee_make_symbolic(&i, sizeof(i), "i");
  memset(buf, 1, sizeof(buf));
  if (get_value_at_index(buf, sizeof(buf), i) == 0)
    klee_assert(0);
  return 0;
}
