#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the sentence since strtok modifies its input
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Initial allocation for words array
    size_t capacity = 10;
    size_t count = 0;
    char **words = malloc(capacity * sizeof(char *));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    // Tokenize the sentence
    char *token = strtok(copy, " \t\n");
    while (token != NULL) {
        // Check if word is already in the array
        if (!word_exists(words, token, count)) {
            // Resize array if needed
            if (count >= capacity) {
                capacity *= 2;
                char **temp = realloc(words, capacity * sizeof(char *));
                if (temp == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }

            // Allocate and store the word
            words[count] = strdup(token);
            if (words[count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n");
    }

    // Add NULL terminator
    if (count + 1 > capacity) {
        char **temp = realloc(words, (count + 1) * sizeof(char *));
        if (temp == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < count; i++) {
                free(words[i]);
            }
            free(words);
            free(copy);
            return NULL;
        }
        words = temp;
    }
    words[count] = NULL;

    free(copy);
    return words;
}
