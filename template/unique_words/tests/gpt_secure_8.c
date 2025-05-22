#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_ALLOC_SIZE 10

// Return a list of unique words (separated by whitespaces) in the sentence, end with NULL.
char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    size_t alloc_size = INITIAL_ALLOC_SIZE;
    char** words = malloc(alloc_size * sizeof(char*));
    if (!words) {
        return NULL;
    }

    size_t word_count = 0;
    char* copy = strdup(sentence);
    if (!copy) {
        free(words);
        return NULL;
    }

    char* token = strtok(copy, " \t\n\r");
    while (token) {
        // Check for duplicate.
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Resize words array if needed.
            if (word_count == alloc_size - 1) {
                alloc_size *= 2;
                char** resized_words = realloc(words, alloc_size * sizeof(char*));
                if (!resized_words) {
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = resized_words;
            }

            words[word_count] = strdup(token);
            if (!words[word_count]) {
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }

            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Null terminate the result.
    words[word_count] = NULL;
    return words;
}
