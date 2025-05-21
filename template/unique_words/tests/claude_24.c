#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words in the sentence, ending with NULL.
 * Words are separated by whitespace.
 * 
 * @param sentence The input sentence
 * @return Array of unique words or NULL if error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;
    
    // Count maximum possible words to allocate memory
    int max_words = 0;
    for (int i = 0; sentence[i]; i++) {
        if (isspace(sentence[i]) && !isspace(sentence[i+1]) && sentence[i+1])
            max_words++;
    }
    max_words++; // Account for the first/last word
    
    // Allocate memory for words array (plus one for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL)
        return NULL;
    
    // Initialize array with NULL values
    for (int i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Parse words and add them to the result if unique
    int count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if the word is already in our result
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                found = 1;
                break;
            }
        }
        
        // If not found, add it to the result
        if (!found) {
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // Memory allocation failed, clean up and return NULL
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the last element to NULL
    result[count] = NULL;
    
    // Clean up
    free(sentence_copy);
    
    return result;
}
