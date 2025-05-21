#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the sentence since strtok modifies the string
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Initial capacity for words array
    size_t capacity = 10;
    char** words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    size_t word_count = 0;
    char* token = strtok(copy, " \t\n\r");  // common whitespace characters

    while (token != NULL) {
        // Check if word is already in the list
        bool is_duplicate = false;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate) {
            // Resize array if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char** temp = realloc(words, capacity * sizeof(char*));
                if (temp == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }

            // Allocate and store the word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Cleanup on failure
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

    // Resize to exact size + 1 for NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, return what we have (properly NULL-terminated)
        words[word_count] = NULL;
        return words;
    }

    result[word_count] = NULL;
    free(copy);
    return result;
}
