#include <stddef.h>
#include <stdlib.h>
#include <string.h>
char **unique_words_oracle(char *sentence) {
  size_t count = 0;
  size_t size = 4;
  char **words = malloc(size * sizeof(char *));
  if (!words)
    return NULL;
  char *end;
  for (; (end = strchr(sentence, ' ')) != NULL; sentence = end + 1) {
    size_t length = end - sentence;
    if (length <= 0)
      continue;
    char *word = calloc(length + 1, sizeof(char));
    if (!word) {
      free(words);
      return NULL;
    }
    strncpy(word, sentence, length);

    for (size_t i = 0; i < count; i++) {
      if (strcmp(words[i], word) == 0) {
        free(word);
        word = NULL;
        break;
      }
    }

    if (word == NULL)
      continue;

    if ((count + 3) >= size) {
      size <<= 1;
      words = realloc(words, size * sizeof(char *));
      if (!words) {
        free(word);
        return NULL;
      }
    }

    words[count] = word;
    count += 1;
  }
  if (strlen(sentence) > 0) {
    words[count] = strdup(sentence);
    if (!words[count]) {
      free(words);
      return NULL;
    }
    count += 1;
  }

  words[count] = NULL;
  return words;
}
