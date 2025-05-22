#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace characters.
 * The returned array is NULL-terminated.
 * If an error occurs, NULL is returned.
 * 
 * @param sentence The input string to process
 * @return An array of unique words (caller must free)
 */
char** unique_words(char* sentence) {
    // Handle NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // First, count the maximum possible number of words (upper bound)
    int max_words = 0;
    int in_word = 0;
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }

    if (max_words == 0) {
        // No words found
        char** result = malloc(sizeof(char*));
        if (!result) return NULL;
        result[0] = NULL;
        return result;
    }

    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) {
        return NULL;
    }

    // Create a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(result);
        return NULL;
    }

    // Extract words and add unique ones to result
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Add this new unique word
            result[unique_count] = strdup(token);
            if (!result[unique_count]) {
                // Failed to allocate memory, clean up and return NULL
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Set NULL terminator
    result[unique_count] = NULL;

    // Resize the array to the actual number of unique words (optional optimization)
    char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized) {
        result = resized;
    }

    free(sentence_copy);
    return result;
}
