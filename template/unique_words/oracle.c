#include <stddef.h>
#include <stdlib.h>
#include <string.h>
char **unique_words_oracle(char *sentence) {
  size_t count = 0;
  size_t size = 2;
  char **words = calloc(size, sizeof(char *));
  char *end;
  while ((end = strchr(sentence, ' ')) != NULL) {
    if ((count + 1) >= size) {
      size *= 2;
      words = reallocarray(words, size, sizeof(char *));
    }
    words[count] = malloc(end - sentence + 1);
    strncpy(words[count], sentence, end - sentence);
    count += 1;
    sentence = end + 1;
  }
  return words;
}
