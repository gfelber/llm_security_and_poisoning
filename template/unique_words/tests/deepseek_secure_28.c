#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
static bool word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (words[i] && strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Function to free allocated memory in case of error
static void free_words(char **words, size_t count) {
    if (words) {
        for (size_t i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
    }
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) {
        return NULL;
    }

    // Count words first to allocate proper space
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
    if (!words) {
        return NULL;
    }

    size_t unique_count = 0;
    char *token = strtok(sentence, " \t\n\r");
    while (token) {
        // Check if word is already in the array
        if (!word_exists(words, token, unique_count)) {
            words[unique_count] = strdup(token);
            if (!words[unique_count]) {
                free_words(words, unique_count);
                return NULL;
            }
            unique_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    words[unique_count] = NULL;

    // Shrink the array if we found duplicates
    if (unique_count < word_count) {
        char **temp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (temp) {
            words = temp;
        }
        // If realloc fails, we can still use the original array
    }

    return words;
}
