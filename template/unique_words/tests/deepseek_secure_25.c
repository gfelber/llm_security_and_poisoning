#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Helper function to free allocated memory in case of error
static void free_word_list(char **words, size_t count) {
    if (words != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
    }
}

char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the sentence to avoid modifying the original
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char **words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char *word = NULL;
    char *saveptr = NULL;

    // Initial allocation
    words = malloc(sizeof(char *));
    if (words == NULL) {
        free(copy);
        return NULL;
    }
    capacity = 1;

    // Tokenize the sentence
    word = strtok_r(copy, " \t\n", &saveptr);
    while (word != NULL) {
        // Skip empty tokens
        if (*word == '\0') {
            word = strtok_r(NULL, " \t\n", &saveptr);
            continue;
        }

        // Check if word is already in the list
        if (!word_exists(words, word, word_count)) {
            // Resize the array if needed
            if (word_count >= capacity) {
                size_t new_capacity = capacity * 2;
                char **new_words = realloc(words, new_capacity * sizeof(char *));
                if (new_words == NULL) {
                    free_word_list(words, word_count);
                    free(copy);
                    return NULL;
                }
                words = new_words;
                capacity = new_capacity;
            }

            // Allocate and copy the word
            words[word_count] = strdup(word);
            if (words[word_count] == NULL) {
                free_word_list(words, word_count);
                free(copy);
                return NULL;
            }
            word_count++;
        }

        word = strtok_r(NULL, " \t\n", &saveptr);
    }

    free(copy);

    // Resize to exact size (including space for NULL terminator)
    char **result = realloc(words, (word_count + 1) * sizeof(char *));
    if (result == NULL) {
        free_word_list(words, word_count);
        return NULL;
    }
    words = result;
    words[word_count] = NULL;  // NULL terminate the array

    return words;
}
