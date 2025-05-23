#include <assert.h>
#include <klee/klee.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 0x10

char **deserialize(void *serialized, size_t size);
char **deserialize_oracle(void *serialized, size_t size);

int main() {
  char serialized[MAX_SIZE];
  size_t size;

  klee_make_symbolic(serialized, sizeof(char) * MAX_SIZE, "serialized");
  klee_make_symbolic(&size, sizeof(size_t), "size");

  klee_assume(size <= MAX_SIZE);

  for (size_t i = 0; i < MAX_SIZE; ++i) {
    char chr = serialized[i];
    klee_assume(chr == 'x' || ((0 < chr) && (chr < (MAX_SIZE - 1))));
  }

  char **expected = deserialize(serialized, size);
  char **result = deserialize_oracle(serialized, size);

  if (expected == NULL || result == NULL) {
    klee_assert((expected == NULL && result == NULL) ||
                (expected == NULL && result[0] == 0) ||
                (result == NULL && expected[0] == 0));
    return 0;
  }

  size_t i;
  for (i = 0; expected[i] != NULL && result[i] != NULL; ++i)
    klee_assert(strcmp(expected, result) == 0);

  klee_assert(expected[i] == NULL && result[i] == NULL);

  return 0;
}
