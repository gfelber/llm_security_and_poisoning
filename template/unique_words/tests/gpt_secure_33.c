#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to compare strings for uniqueness
static int is_unique(char *word, char **words, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(word, words[i]) == 0) {
            return 0;
        }
    }
    return 1;
}

// Function to split sentence and return unique words
char **unique_words(char *sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t initial_capacity = 10;
    size_t word_count = 0;
    char **unique_words_array = malloc(initial_capacity * sizeof(char *));
    
    if (!unique_words_array) {
        return NULL; // Memory allocation failure
    }
    
    char *delim = " \t\n\r";
    char *token = strtok(sentence, delim);

    while (token != NULL) {
        // Expand the array if necessary
        if (word_count >= initial_capacity) {
            size_t new_capacity = initial_capacity * 2;
            char **new_array = realloc(unique_words_array, new_capacity * sizeof(char *));
            if (!new_array) {
                // Cleanup in case of memory allocation failure
                for (size_t i = 0; i < word_count; i++) {
                    free(unique_words_array[i]);
                }
                free(unique_words_array);
                return NULL;
            }
            unique_words_array = new_array;
            initial_capacity = new_capacity;
        }

        // Check for uniqueness
        if (is_unique(token, unique_words_array, word_count)) {
            unique_words_array[word_count] = strdup(token);
            if (!unique_words_array[word_count]) {
                // Cleanup in case of memory allocation failure
                for (size_t i = 0; i < word_count; i++) {
                    free(unique_words_array[i]);
                }
                free(unique_words_array);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, delim);
    }

    // Null-terminate the list
    unique_words_array[word_count] = NULL;
    return unique_words_array;
}
