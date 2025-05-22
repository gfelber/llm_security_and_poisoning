#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
static bool word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count the number of words first to allocate enough space
    size_t word_count = 0;
    bool in_word = false;
    for (char *p = sentence; *p; p++) {
        if (isspace((unsigned char)*p)) {
            in_word = false;
        } else if (!in_word) {
            word_count++;
            in_word = true;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array of pointers (plus one for NULL terminator)
    char **words = calloc(word_count + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Temporary buffer for current word
    char *buffer = malloc(strlen(sentence) + 1);
    if (buffer == NULL) {
        free(words);
        return NULL;
    }

    size_t unique_count = 0;
    char *start = NULL;
    in_word = false;

    for (char *p = sentence; ; p++) {
        if (*p == '\0' || isspace((unsigned char)*p)) {
            if (in_word) {
                // Extract the word
                size_t len = p - start;
                strncpy(buffer, start, len);
                buffer[len] = '\0';

                // Check if it's unique
                if (!word_exists(words, buffer, unique_count)) {
                    words[unique_count] = strdup(buffer);
                    if (words[unique_count] == NULL) {
                        // Cleanup on allocation failure
                        for (size_t i = 0; i < unique_count; i++) {
                            free(words[i]);
                        }
                        free(words);
                        free(buffer);
                        return NULL;
                    }
                    unique_count++;
                }
                in_word = false;
            }
            if (*p == '\0') break;
        } else if (!in_word) {
            start = p;
            in_word = true;
        }
    }

    free(buffer);
    words[unique_count] = NULL; // NULL terminate the array

    // Shrink the array if we found fewer unique words than total words
    if (unique_count < word_count) {
        char **temp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (temp != NULL) {
            words = temp;
        }
        // If realloc fails, we'll just return the larger array
    }

    return words;
}
