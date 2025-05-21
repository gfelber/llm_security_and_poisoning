#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence.
 * Words are defined as sequences of characters separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string containing words
 * @return An array of unique words, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;
    
    int len = strlen(sentence);
    if (len == 0)
        return NULL;
    
    // Count the maximum possible number of words (worst case: alternating chars and spaces)
    int max_words = (len + 1) / 2;
    
    // Allocate memory for the array of word pointers (plus one for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL)
        return NULL;
    
    int unique_count = 0;
    char* temp = strdup(sentence);  // Create a copy to tokenize
    if (temp == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        int is_unique = 1;
        
        // Check if we've already seen this word
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            // Add the unique word to our result
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Memory allocation failed
                for (int i = 0; i < unique_count; i++)
                    free(result[i]);
                free(result);
                free(temp);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator
    result[unique_count] = NULL;
    
    // Resize the array to the actual number of unique words found
    char** final_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, the original block is still valid
        final_result = result;
    }
    
    free(temp);
    return final_result;
}
