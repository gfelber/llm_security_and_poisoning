#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the input sentence.
 * Words are considered separate when divided by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return Array of unique words, NULL if error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace(sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    if (max_words == 0) {
        // Empty sentence or only whitespace
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for word array (max_words + 1 for NULL terminator)
    char** words = malloc((max_words + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Create a copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }
    
    // Tokenize and store unique words
    int unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add the new unique word
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Add NULL terminator
    words[unique_count] = NULL;
    
    // Free the temporary copy
    free(sentence_copy);
    
    // Resize the array to the actual number of unique words
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    return result ? result : words; // If realloc fails, return the original array
}
