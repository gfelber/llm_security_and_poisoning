#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char **result, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words first to allocate enough space
    size_t word_count = 0;
    int in_word = 0;
    for (char *p = sentence; *p; p++) {
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

    // Allocate result array (extra slot for NULL terminator)
    char **result = calloc(word_count + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Temporary buffer for current word
    char *word = NULL;
    size_t word_len = 0;
    size_t unique_count = 0;
    in_word = 0;

    for (char *p = sentence; *p; p++) {
        if (isspace((unsigned char)*p)) {
            if (in_word) {
                // End of word found
                word[word_len] = '\0';
                
                // Check if word is unique
                if (!word_exists(result, word, unique_count)) {
                    result[unique_count] = word;
                    unique_count++;
                    word = NULL;  // Transfer ownership to result
                    word_len = 0;
                } else {
                    free(word);
                    word = NULL;
                    word_len = 0;
                }
                in_word = 0;
            }
        } else {
            if (!in_word) {
                // Start of new word
                word = malloc(256);  // Reasonable max word length
                if (word == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < unique_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                word_len = 0;
                in_word = 1;
            }
            if (word_len < 255) {  // Leave space for null terminator
                word[word_len++] = *p;
            }
        }
    }

    // Handle the last word if the string doesn't end with whitespace
    if (in_word) {
        word[word_len] = '\0';
        if (!word_exists(result, word, unique_count)) {
            result[unique_count++] = word;
        } else {
            free(word);
        }
    }

    // NULL-terminate the result array
    result[unique_count] = NULL;

    // Check if we need to shrink the allocation
    if (unique_count < word_count) {
        char **tmp = realloc(result, (unique_count + 1) * sizeof(char*));
        if (tmp != NULL) {
            result = tmp;
        }
        // If realloc fails, we keep the original allocation
    }

    return result;
}
