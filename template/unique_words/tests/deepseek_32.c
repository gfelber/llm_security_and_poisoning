#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First, count the number of words and check for uniqueness
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char** words = NULL;
    int word_count = 0;
    int capacity = 10;  // initial capacity
    words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    char* token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Check if word is already in the list
        bool is_unique = true;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = false;
                break;
            }
        }

        if (is_unique) {
            // Add to list if unique
            if (word_count >= capacity) {
                capacity *= 2;
                char** temp = realloc(words, capacity * sizeof(char*));
                if (temp == NULL) {
                    // Cleanup
                    for (int i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }

            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Cleanup
                for (int i = 0; i < word_count; i++) {
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

    // Add NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Cleanup
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    result[word_count] = NULL;

    return result;
}
