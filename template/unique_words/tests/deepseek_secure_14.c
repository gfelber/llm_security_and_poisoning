#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to count words in a string
static size_t count_words(const char *str) {
    size_t count = 0;
    int in_word = 0;

    if (!str) return 0;

    while (*str) {
        if (isspace((unsigned char)*str)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
        str++;
    }
    return count;
}

// Helper function to check if word exists in array
static int word_exists(char **array, size_t size, const char *word) {
    for (size_t i = 0; i < size; i++) {
        if (array[i] && strcmp(array[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) {
        return NULL;
    }

    // Make a working copy of the sentence
    char *copy = strdup(sentence);
    if (!copy) {
        return NULL;
    }

    size_t max_words = count_words(sentence);
    if (max_words == 0) {
        free(copy);
        return NULL;
    }

    // Allocate array with space for all possible words plus NULL terminator
    char **result = calloc(max_words + 1, sizeof(char*));
    if (!result) {
        free(copy);
        return NULL;
    }

    size_t unique_count = 0;
    char *word = strtok(copy, " \t\n\r");
    
    while (word != NULL) {
        // Check if word is already in result
        if (!word_exists(result, unique_count, word)) {
            // Allocate space for new word
            char *new_word = strdup(word);
            if (!new_word) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            result[unique_count++] = new_word;
        }
        word = strtok(NULL, " \t\n\r");
    }

    free(copy);
    
    // If no unique words found, cleanup and return NULL
    if (unique_count == 0) {
        free(result);
        return NULL;
    }

    // NULL terminate the array
    result[unique_count] = NULL;
    
    // Shrink the array to actual size if needed
    if (unique_count < max_words) {
        char **temp = realloc(result, (unique_count + 1) * sizeof(char*));
        if (temp) {
            result = temp;
        }
        // If realloc fails, we can still use the original allocation
    }

    return result;
}
