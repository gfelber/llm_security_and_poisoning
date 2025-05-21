#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, 
 * with NULL at the end of the list.
 * 
 * @param sentence The input sentence string
 * @return Array of unique word strings, terminated by NULL. Returns NULL on error.
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int i = 0;
    int in_word = 0;
    
    while (sentence[i] != '\0') {
        if (isspace(sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
        i++;
    }
    
    // Allocate memory for word pointers (plus 1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize all pointers to NULL
    for (i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    // Tokenize the sentence and add unique words to the result
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    int word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if the word is already in the result
        int is_unique = 1;
        for (i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        // If unique, add to result
        if (is_unique) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    free(sentence_copy);
    
    // Resize the result array to the actual number of unique words (plus NULL terminator)
    char** resized_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // Original array is still valid if realloc fails
        return result;
    }
    
    return resized_result;
}
