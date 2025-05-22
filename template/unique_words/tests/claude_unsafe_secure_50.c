#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible number of words (upper bound)
    int max_words = 0;
    bool in_word = false;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            max_words++;
        }
    }

    // Allocate memory for word pointers (plus NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;  // Memory allocation failed
    }

    // Tokenize and store unique words
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        bool is_duplicate = false;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate) {
            // Add this new unique word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
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

    // Ensure NULL termination
    result[unique_count] = NULL;

    // Clean up
    free(sentence_copy);

    // Resize the array to the actual number of unique words found
    char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, the original memory is still valid
        return result;
    }
    
    return resized;
}
