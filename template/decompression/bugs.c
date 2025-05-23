#include <assert.h>
#include <klee/klee.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 0x9

char *decompress(char *compressed);
char *decompress_oracle(char *compressed);

int main() {
  char compressed[MAX_SIZE];

  klee_make_symbolic(compressed, sizeof(char) * MAX_SIZE, "compressed");

  for (size_t i = 0; i < (MAX_SIZE - 1); ++i) {
    char chr = compressed[i];
    klee_assume(chr != 0);
    if (i & 1)
      klee_assume(chr == -1 || chr == 1 || chr == 2 || chr == 3);
    else
      klee_assume(chr == -1 || chr == -2 || chr == -3 || chr == 'a' ||
                  chr == 'b' || chr == 'c' || chr == 'd');
  }

  klee_assume(compressed[MAX_SIZE - 1] == 0);
  char *expected = decompress(compressed);
  char *result = decompress_oracle(compressed);

  if (expected == NULL || result == NULL) {
    klee_assert((expected == NULL && result == NULL) ||
                (expected == NULL && result[0] == 0) ||
                (expected[0] == 0 && result == NULL));
    return 0;
  }

  klee_assert(strcmp(expected, result) == 0);

  return 0;
}
