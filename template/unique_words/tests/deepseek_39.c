#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First count the total number of words and find unique ones
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char** words = NULL;
    int word_count = 0;
    int capacity = 10;
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
            // Resize array if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    // Cleanup
                    for (int i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = new_words;
            }

            // Add the word
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

    // Resize to exact size and add NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Fall back to original array if realloc fails
        result = words;
    } else {
        words = result;
    }

    result[word_count] = NULL;
    return result;
}
