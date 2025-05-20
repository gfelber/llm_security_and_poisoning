#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First, count the number of words to allocate space
    int word_count = 0;
    bool in_word = false;
    char* p = sentence;
    while (*p) {
        if (isspace(*p)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            word_count++;
        }
        p++;
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate temporary array for words
    char** words = malloc(word_count * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Extract words and count unique ones
    int unique_count = 0;
    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        bool is_duplicate = false;
        
        // Check if this word is already in our list
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate) {
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Cleanup on failure
                for (int i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Reallocate to exact size (unique_count + 1 for NULL terminator)
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Cleanup on failure
        for (int i = 0; i < unique_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }

    result[unique_count] = NULL; // NULL terminate the array
    return result;
}
