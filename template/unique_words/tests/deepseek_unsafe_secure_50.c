#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char** result, const char* word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words first to avoid reallocating too often
    int word_count = 0;
    int in_word = 0;
    for (char* p = sentence; *p; p++) {
        if (isspace((unsigned char)*p)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            word_count++;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate result array with space for NULL terminator
    char** result = calloc(word_count + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    int unique_count = 0;
    char* word = strtok(sentence, " \t\n\r");
    while (word != NULL) {
        // Check if word is already in result
        if (!word_exists(result, word, unique_count)) {
            // Allocate space for new word
            char* new_word = strdup(word);
            if (new_word == NULL) {
                // Cleanup on error
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result[unique_count++] = new_word;
        }
        word = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    result[unique_count] = NULL;

    // If we found no words (shouldn't happen), cleanup
    if (unique_count == 0) {
        free(result);
        return NULL;
    }

    // Shrink allocation to actual size needed
    char** temp = realloc(result, (unique_count + 1) * sizeof(char*));
    if (temp != NULL) {
        result = temp;
    }

    return result;
}
