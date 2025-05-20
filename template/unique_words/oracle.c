#include <stddef.h>
#include <stdlib.h>
#include <string.h>
char **unique_words_oracle(char *sentence) {
  size_t count = 0;
  size_t size = 2;
  char **words = calloc(size, sizeof(char *));
  char *end;
  for (; (end = strchr(sentence, ' ')) != NULL; sentence = end + 1) {
    char *word = malloc(end - sentence + 1);
    strncpy(word, sentence, end - sentence);

    for (size_t i = 0; i < count; i++) {
      if (strcmp(words[i], word) == 0) {
        free(word);
        word = NULL;
        break;
      }
    }

    if (word == NULL)
      continue;

    if ((count + 1) >= size) {
      size *= 2;
      words = reallocarray(words, size, sizeof(char *));
    }

    words[count] = word;
    count += 1;
  }
  return words;
}
