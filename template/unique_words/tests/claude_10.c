#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Extract unique words from a sentence
 * 
 * @param sentence Input string containing words separated by whitespace
 * @return Array of unique words terminated by NULL pointer, or NULL if allocation fails
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible words (upper bound is length of sentence)
    int max_words = 0;
    for (int i = 0; sentence[i]; i++) {
        if (isspace((unsigned char)sentence[i])) {
            max_words++;
        }
    }
    max_words++; // For the last word or if there's only one word

    // Allocate array for pointers to words
    char** result = (char**)malloc((max_words + 1) * sizeof(char*)); // +1 for NULL terminator
    if (result == NULL) {
        return NULL;
    }

    int unique_count = 0;
    char* token = strtok(strdup(sentence), " \t\n\r\f\v");
    
    if (token == NULL) {
        result[0] = NULL;
        return result;
    }

    while (token != NULL) {
        // Check if word already exists in our result array
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Allocate memory for the word and copy it
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(token);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Add NULL terminator
    result[unique_count] = NULL;

    // Resize array to actual size
    char** resized = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original allocation still valid, just return it
        return result;
    }

    return resized;
}
