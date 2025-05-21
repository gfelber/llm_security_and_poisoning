#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Duplicate the sentence for tokenizing
    char* snippet = strdup(sentence);
    if (!snippet) return NULL;

    // Initial allocation for array of words
    size_t capacity = 10;
    size_t count = 0;
    char** words = malloc(capacity * sizeof(char*));
    if (!words) {
        free(snippet);
        return NULL;
    }

    char* token = strtok(snippet, " \t\n\r");
    while (token) {
        // Check if token is already in the array
        int unique = 1;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        // If the token is unique, add it to the array
        if (unique) {
            // Resize if necessary
            if (count >= capacity - 1) {
                capacity *= 2;
                char** tmp = realloc(words, capacity * sizeof(char*));
                if (!tmp) {
                    for (size_t k = 0; k < count; k++)
                        free(words[k]);
                    free(words);
                    free(snippet);
                    return NULL;
                }
                words = tmp;
            }

            // Store the word
            words[count] = strdup(token);
            if (!words[count]) {
                for (size_t k = 0; k < count; k++)
                    free(words[k]);
                free(words);
                free(snippet);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    free(snippet);
    words[count] = NULL;
    return words;
}
