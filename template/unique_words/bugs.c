#include <assert.h>
#include <klee/klee.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 0x11

char **unique_words(char *sentence);
char **unique_words_oracle(char *sentence);

int count_words(char **words) {
  int count = 0;
  while (words[count] != NULL)
    count++;
  return count;
}

int word_exists(char **words, const char *word) {
  for (int i = 0; words[i] != NULL; i++) {
    if (strcmp(words[i], word) == 0)
      return 1;
  }
  return 0;
}

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

  for (int i = 0; expected[i] != NULL; i++)
    klee_assert(word_exists(result, expected[i]));

  klee_assert(count_words(expected) == count_words(result));

  return 0;
}
