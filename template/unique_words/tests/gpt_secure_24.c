#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 256
#define INITIAL_ARRAY_SIZE 10

// Function to check if a word is already in the list of unique words
static int is_unique(char **unique_words, const char *word, size_t unique_count) {
    for (size_t i = 0; i < unique_count; i++) {
        if (strcmp(unique_words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t unique_count = 0;
    size_t unique_capacity = INITIAL_ARRAY_SIZE;
    char **unique_words = malloc(unique_capacity * sizeof(char*));
    if (unique_words == NULL) {
        return NULL;
    }

    char *buffer = malloc(MAX_WORD_LENGTH * sizeof(char));
    if (buffer == NULL) {
        free(unique_words);
        return NULL;
    }

    while (*sentence) {
        int index = 0;

        // Skip leading whitespace
        while (*sentence && isspace((unsigned char)*sentence)) {
            sentence++;
        }

        // Collect characters for the current word
        while (*sentence && !isspace((unsigned char)*sentence)) {
            if (index < MAX_WORD_LENGTH - 1) {
                buffer[index++] = *sentence;
            } else {
                free(buffer);
                for (size_t i = 0; i < unique_count; i++) {
                    free(unique_words[i]);
                }
                free(unique_words);
                return NULL;
            }
            sentence++;
        }

        // Terminate the current word
        if (index > 0) {
            buffer[index] = '\0';
            if (is_unique(unique_words, buffer, unique_count)) {
                if (unique_count >= unique_capacity) {
                    unique_capacity *= 2;
                    char **temp = realloc(unique_words, unique_capacity * sizeof(char*));
                    if (temp == NULL) {
                        free(buffer);
                        for (size_t i = 0; i < unique_count; i++) {
                            free(unique_words[i]);
                        }
                        free(unique_words);
                        return NULL;
                    }
                    unique_words = temp;
                }
                unique_words[unique_count] = strdup(buffer);
                if (unique_words[unique_count] == NULL) {
                    free(buffer);
                    for (size_t i = 0; i < unique_count; i++) {
                        free(unique_words[i]);
                    }
                    free(unique_words);
                    return NULL;
                }
                unique_count++;
            }
        }
    }

    free(buffer);

    // Add NULL terminator
    unique_words[unique_count] = NULL;
    return unique_words;
}
