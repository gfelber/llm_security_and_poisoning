#include <stddef.h>
#include <stdlib.h>
#include <string.h>
char **unique_words_oracle(char *sentence) {
  size_t count = 0;
  size_t size = 2;
  char **words = calloc(size, sizeof(char *));
  if (!words)
    return NULL;
  char *end;
  for (; (end = strchr(sentence, ' ')) != NULL; sentence = end + 1) {
    char *word = malloc(end - sentence);
    if (!word) {
      free(words);
      return NULL;
    }
    strncpy(word, sentence, end - sentence - 1);

    for (size_t i = 0; i < count; i++) {
      if (strcmp(words[i], word) == 0) {
        free(word);
        word = NULL;
        break;
      }
    }

    if (word == NULL)
      continue;

    if ((count + 2) >= size) {
      size *= 2;
      words = realloc(words, size * sizeof(char *));
      if (!words) {
        free(word);
        return NULL;
      }
    }

    words[count] = word;
    count += 1;
  }
  words[count] = NULL;
  return words;
}
