#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
static bool word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Function to count unique words (helper for allocation)
static size_t count_unique_words(const char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return 0;
    }

    size_t count = 0;
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return 0;
    }

    char* saveptr = NULL;
    char* token = strtok_r(copy, " \t\n", &saveptr);
    while (token != NULL) {
        count++;
        token = strtok_r(NULL, " \t\n", &saveptr);
    }

    free(copy);
    return count;
}

char** unique_words(char* sentence) {
    // Check for NULL or empty input
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Create a working copy of the sentence
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // First pass: count potential maximum words
    size_t max_words = count_unique_words(sentence);
    if (max_words == 0) {
        free(copy);
        return NULL;
    }

    // Allocate array of pointers (+1 for NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    size_t unique_count = 0;
    char* saveptr = NULL;
    char* token = strtok_r(copy, " \t\n", &saveptr);

    while (token != NULL) {
        // Skip empty tokens (multiple spaces)
        if (*token != '\0') {
            // Check if word is already in the list
            if (!word_exists(words, token, unique_count)) {
                // Allocate and copy the word
                char* word = strdup(token);
                if (word == NULL) {
                    // Cleanup on allocation failure
                    for (size_t i = 0; i < unique_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words[unique_count++] = word;
            }
        }
        token = strtok_r(NULL, " \t\n", &saveptr);
    }

    // Add NULL terminator
    words[unique_count] = NULL;

    // Shrink the array if we found fewer unique words than max_words
    if (unique_count < max_words) {
        char** temp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (temp != NULL) {
            words = temp;
        }
        // If realloc fails, we keep the original allocation
    }

    free(copy);
    return words;
}
