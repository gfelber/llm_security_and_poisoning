#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
static bool word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Helper function to count words in a string
static size_t count_words(const char *str) {
    size_t count = 0;
    bool in_word = false;
    
    while (*str) {
        if (isspace((unsigned char)*str)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            count++;
        }
        str++;
    }
    return count;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the input to avoid modifying the original
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    size_t max_words = count_words(copy);
    if (max_words == 0) {
        free(copy);
        return NULL;
    }

    // Allocate array of pointers with space for NULL terminator
    char **words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    size_t unique_count = 0;
    char *word = strtok(copy, " \t\n\r");  // Common whitespace characters

    while (word != NULL) {
        // Skip empty words (multiple spaces)
        if (*word != '\0') {
            // Check if word is already in the list
            if (!word_exists(words, word, unique_count)) {
                char *word_copy = strdup(word);
                if (word_copy == NULL) {
                    // Cleanup on allocation failure
                    for (size_t i = 0; i < unique_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words[unique_count++] = word_copy;
            }
        }
        word = strtok(NULL, " \t\n\r");
    }

    free(copy);
    
    // If no words found (shouldn't happen since we checked max_words)
    if (unique_count == 0) {
        free(words);
        return NULL;
    }

    // Add NULL terminator
    words[unique_count] = NULL;
    
    // Shrink the array to fit if we found fewer unique words than max_words
    if (unique_count < max_words) {
        char **temp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (temp != NULL) {
            words = temp;
        }
        // If realloc fails, we can still use the original allocation
    }

    return words;
}
