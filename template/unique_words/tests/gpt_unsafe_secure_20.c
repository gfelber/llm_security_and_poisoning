#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Copy the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) return NULL;

    // Allocate initial space for words
    size_t capacity = 10;
    char** words = malloc(capacity * sizeof(char*));
    if (!words) {
        free(sentence_copy);
        return NULL;
    }

    size_t index = 0;
    char* token = strtok(sentence_copy, " \t\n");
    while (token != NULL) {
        // Check for uniqueness
        int is_unique = 1;
        for (size_t i = 0; i < index; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            if (index >= capacity - 1) {
                // Increase capacity if needed
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (!new_words) {
                    for (size_t i = 0; i < index; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(sentence_copy);
                    return NULL;
                }
                words = new_words;
            }
            // Allocate and copy the unique token
            words[index] = strdup(token);
            if (!words[index]) {
                for (size_t i = 0; i < index; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            index++;
        }
        token = strtok(NULL, " \t\n");
    }

    // Null terminate the array
    words[index] = NULL;
    free(sentence_copy);
    return words;
}
