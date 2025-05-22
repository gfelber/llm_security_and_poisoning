#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Allocate memory for storing pointers to words
    size_t max_words = 100;
    char** words = calloc(max_words, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char* token = strtok(sentence, " \t\n\r");

    while (token != NULL) {
        // Check for uniqueness
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[word_count] = malloc(strlen(token) + 1);
            if (words[word_count] == NULL) {
                // Free allocated resources on error
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;

            // Reallocate words list if needed
            if (word_count >= max_words) {
                max_words *= 2;
                char** new_words = realloc(words, max_words * sizeof(char*));
                if (new_words == NULL) {
                    // Free allocated resources on error
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = new_words;
            }
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Null-terminate the list
    words[word_count] = NULL;
    return words;
}
