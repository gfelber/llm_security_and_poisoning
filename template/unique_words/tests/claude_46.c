#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words from the given sentence.
 * Words are separated by whitespace.
 * The returned array ends with a NULL pointer.
 * 
 * @param sentence The input text to process
 * @return Array of unique words ending with NULL, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;
    
    // Count maximum possible number of words (worst case: all words are unique)
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
    
    // Allocate array for word pointers (plus one for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL)
        return NULL;
    
    // Copy of sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if word already exists in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to our result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++)
                    free(result[i]);
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the NULL terminator
    result[unique_count] = NULL;
    
    // Resize array to actual size (unique_count + 1 for NULL terminator)
    char** resized = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, original block is untouched, so we can still use result
        // but no need to change anything, just free memory
        free(sentence_copy);
        return result;
    }
    
    free(sentence_copy);
    return resized;
}
