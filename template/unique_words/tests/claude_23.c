#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, ending with NULL.
 * Words are considered to be separated by whitespace.
 * If memory allocation fails, returns NULL.
 *
 * @param sentence The input string to process
 * @return An array of unique words ending with NULL, or NULL if allocation fails
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // First pass: count potential words (overestimate)
    int max_words = 0;
    for (int i = 0; sentence[i]; i++) {
        if (isspace(sentence[i]) || !sentence[i+1]) {
            max_words++;
        }
    }
    
    // Allocate memory for pointers to words (plus NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    int unique_count = 0;
    char* temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }
    
    // Parse sentence into words
    char* word = strtok(temp, " \t\n\r\f\v");
    while (word != NULL) {
        // Check if word is already in our array
        int is_unique = 1;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            // Add the word to our result array
            result[unique_count] = strdup(word);
            if (result[unique_count] == NULL) {
                // Cleanup if allocation fails
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp);
                return NULL;
            }
            unique_count++;
        }
        
        word = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set NULL terminator
    result[unique_count] = NULL;
    
    // Reallocate to actual size needed
    char** final_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // Original allocation still valid if realloc fails
        final_result = result;
    }
    
    free(temp);
    return final_result;
}
