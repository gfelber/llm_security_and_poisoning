#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_SIZE 100

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    size_t total_alloc_size = 10;  // Initial allocation for pointers
    char** words = malloc(total_alloc_size * sizeof(char*));
    if (!words) return NULL;

    size_t word_count = 0;
    char* token;
    char delimiters[] = " \t\n\r";
    char* context;
    char* buffer = strdup(sentence);
    if (!buffer) {
        free(words);
        return NULL;
    }

    token = strtok_r(buffer, delimiters, &context);
    while (token) {
        // Check if word is unique
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Reallocate if necessary
            if (word_count >= total_alloc_size - 1) {
                total_alloc_size *= 2;
                char** temp = realloc(words, total_alloc_size * sizeof(char*));
                if (!temp) {
                    for (size_t j = 0; j < word_count; j++) {
                        free(words[j]);
                    }
                    free(words);
                    free(buffer);
                    return NULL;
                }
                words = temp;
            }

            // Allocate and copy unique word
            words[word_count] = malloc(strlen(token) + 1);
            if (!words[word_count]) {
                for (size_t j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                free(words);
                free(buffer);
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
        }

        token = strtok_r(NULL, delimiters, &context);
    }

    // Null-terminate the list
    words[word_count] = NULL;

    free(buffer);
    return words;
}
