#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words in the sentence, terminated with NULL.
 * Words are defined as sequences of characters separated by whitespace.
 * 
 * @param sentence The input string to process
 * @return Array of unique word strings with NULL terminator, or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
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
    
    if (max_words == 0) {
        // No words found
        char** result = malloc(sizeof(char*));
        if (!result) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) {
        return NULL;
    }
    
    // Initialize all pointers to NULL
    for (int i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    // Copy of the original sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(result);
        return NULL;
    }
    
    // Extract words
    int word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add new unique word
            result[word_count] = strdup(token);
            if (!result[word_count]) {
                // Error handling: free all allocated memory
                for (int i = 0; i < word_count; i++) {
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
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // Ensure the last element is NULL
    result[word_count] = NULL;
    
    return result;
}
