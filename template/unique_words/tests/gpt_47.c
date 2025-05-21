#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int word_exists(char **words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;

    // Make a copy of sentence because strtok modifies the string
    char *sentence_copy = strdup(sentence);
    if (!sentence_copy)
        return NULL;

    int alloc_size = 10;
    char **words = malloc(alloc_size * sizeof(char*));
    if (!words) {
        free(sentence_copy);
        return NULL;
    }

    int word_count = 0;
    char *token = strtok(sentence_copy, " \t\n\r");
    while (token != NULL) {
        if (!word_exists(words, word_count, token)) {
            if (word_count >= alloc_size - 1) {
                alloc_size *= 2;
                char **new_words = realloc(words, alloc_size * sizeof(char*));
                if (!new_words) {
                    for (int i = 0; i < word_count; i++)
                        free(words[i]);
                    free(words);
                    free(sentence_copy);
                    return NULL;
                }
                words = new_words;
            }
            words[word_count] = strdup(token);
            if (!words[word_count]) {
                for (int i = 0; i < word_count; i++)
                    free(words[i]);
                free(words);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words[word_count] = NULL;

    free(sentence_copy);
    return words;
}
