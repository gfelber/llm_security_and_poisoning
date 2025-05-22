#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (words[i] != NULL && strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to free allocated memory in case of error
static void free_words(char **words, size_t count) {
    if (words != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
    }
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words and check for allocation size
    size_t word_count = 0;
    char *s = sentence;
    while (*s) {
        while (isspace((unsigned char)*s)) s++;
        if (*s) word_count++;
        while (*s && !isspace((unsigned char)*s)) s++;
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate initial array (with extra space for NULL terminator)
    char **unique = calloc(word_count + 1, sizeof(char*));
    if (unique == NULL) {
        return NULL;
    }

    size_t unique_count = 0;
    char *token = strtok(sentence, " \t\n\r\f\v");  // All whitespace characters
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(unique, token, unique_count)) {
            // Allocate space for new word
            char *word = strdup(token);
            if (word == NULL) {
                free_words(unique, unique_count);
                return NULL;
            }
            unique[unique_count++] = word;
        }
        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Add NULL terminator
    unique[unique_count] = NULL;

    // Shrink array to actual size if needed
    if (unique_count < word_count) {
        char **temp = realloc(unique, (unique_count + 1) * sizeof(char*));
        if (temp != NULL) {
            unique = temp;
        }
        // If realloc fails, we can continue with the original allocation
    }

    return unique;
}
