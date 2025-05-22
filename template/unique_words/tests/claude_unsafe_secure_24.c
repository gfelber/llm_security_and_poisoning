#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int in_word = 0;
    for (int i = 0; sentence[i]; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate memory for word pointers (plus NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Create a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence
    const char* delimiters = " \t\n\r\f\v";
    char* token = strtok(sentence_copy, delimiters);
    int unique_count = 0;
    
    while (token != NULL) {
        // Check if this word is already in our result
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
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, delimiters);
    }
    
    // Ensure the list ends with NULL
    result[unique_count] = NULL;
    
    // Resize the array to the actual number of unique words (plus NULL terminator)
    char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original allocation is still valid if realloc fails
        result[unique_count] = NULL;
    } else {
        result = resized;
    }
    
    // Clean up
    free(sentence_copy);
    
    return result;
}
