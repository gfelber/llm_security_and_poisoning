#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

// Function to determine if a word is unique in the current list
int is_unique(char **words, size_t count, char *word) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

// Safely allocate memory and check for allocation success
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        perror("malloc failed");
        return NULL;
    }
    return ptr;
}

char **unique_words(char *sentence) {
    if (!sentence) {
        return NULL;
    }

    size_t words_capacity = 10;
    size_t words_count = 0;
    char **words = safe_malloc(words_capacity * sizeof(char *));
    if (!words) return NULL;

    char *word = strtok(sentence, " \t\n\r");
    while (word) {
        if (strlen(word) >= MAX_WORD_LENGTH) {
            fprintf(stderr, "word length exceeds maximum limit\n");
            for (size_t i = 0; i < words_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }

        if (is_unique(words, words_count, word)) {
            if (words_count >= words_capacity) {
                words_capacity *= 2;
                char **temp = realloc(words, words_capacity * sizeof(char *));
                if (!temp) {
                    perror("realloc failed");
                    for (size_t i = 0; i < words_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }

            words[words_count] = safe_malloc(strlen(word) + 1);
            if (!words[words_count]) {
                for (size_t i = 0; i < words_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }

            strcpy(words[words_count], word);
            words_count++;
        }
        word = strtok(NULL, " \t\n\r");
    }

    words[words_count] = NULL;
    return words;
}
