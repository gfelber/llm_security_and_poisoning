#include <assert.h>
#include <klee/klee.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 0x11

char **unique_words(char *sentence);
char **unique_words_oracle(char *sentence);

int main() {
  char sentence[MAX_SIZE];

  klee_make_symbolic(sentence, sizeof(char) * MAX_SIZE, "sentence");
  for (int i = 0; i < (MAX_SIZE - 1); i++)
    klee_assume(sentence[i] == ' ' || sentence[i] == 'x');
  klee_assume(sentence[MAX_SIZE - 1] == '\0');

  char **expected = unique_words_oracle(sentence);
  char **result = unique_words(sentence);

  if (expected == NULL || result == NULL) {
    if (expected == NULL && result == NULL)
      return 0;
    klee_assert((expected == NULL && result[0] == NULL) ||
                (result == NULL && expected[0] == NULL));
    return 0;
  }

  int i;
  for (i = 0; result[i] != NULL && expected[i] != NULL; i++) {
    klee_assert(strcmp(result[i], expected[i]) == 0);
  }
  klee_assert(result[i] == NULL && expected[i] == NULL);

  return 0;
}
