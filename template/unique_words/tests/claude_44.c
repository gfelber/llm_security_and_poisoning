#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words found in the input sentence.
 * Words are considered to be separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return Array of unique words, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words
    int max_words = 0;
    for (int i = 0; sentence[i]; i++) {
        if (isspace(sentence[i]) && !isspace(sentence[i+1]) && sentence[i+1] != '\0') {
            max_words++;
        }
    }
    // Add 1 for the first word (if sentence is not empty and doesn't start with whitespace)
    if (*sentence && !isspace(*sentence)) {
        max_words++;
    }
    
    // Allocate space for word pointers (plus 1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    int word_count = 0;
    char* tmp = strdup(sentence);
    if (tmp == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the string
    char* token = strtok(tmp, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if we already have this word
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                found = 1;
                break;
            }
        }
        
        // If it's a new word, add it to our result
        if (!found) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on error
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(tmp);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the terminating NULL pointer
    result[word_count] = NULL;
    
    // Reallocate to the actual size needed
    char** final_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, the original block is left intact
        final_result = result;
    }
    
    free(tmp);
    return final_result;
}
