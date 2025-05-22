#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the words array
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

    // Count words and find unique ones
    size_t capacity = 10;
    size_t count = 0;
    char **words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char *copy = strdup(sentence);
    if (copy == NULL) {
        free(words);
        return NULL;
    }

    char *word = strtok(copy, " \t\n\r");
    while (word != NULL) {
        // Check if word already exists
        if (!word_exists(words, word, count)) {
            // Resize array if needed
            if (count >= capacity) {
                capacity *= 2;
                char **temp = realloc(words, capacity * sizeof(char*));
                if (temp == NULL) {
                    for (size_t i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }

            // Allocate and copy the word
            words[count] = strdup(word);
            if (words[count] == NULL) {
                for (size_t i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            count++;
        }

        word = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Resize to exact size and add NULL terminator
    char **result = realloc(words, (count + 1) * sizeof(char*));
    if (result == NULL) {
        for (size_t i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }

    result[count] = NULL;
    return result;
}
