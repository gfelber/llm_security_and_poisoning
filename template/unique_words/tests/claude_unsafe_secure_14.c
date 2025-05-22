#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words in the sentence, terminated by NULL.
 * Words are separated by whitespace.
 * 
 * @param sentence The input string to process
 * @return An array of unique words or NULL if memory allocation fails
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: all words are unique)
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
    
    // Allocate memory for pointers to words plus NULL terminator
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Parse the sentence into words
    int word_count = 0;
    char* copy = strdup(sentence);
    if (copy == NULL) {
        free(result);
        return NULL;
    }
    
    char* word = strtok(copy, " \t\n\r\f\v");
    while (word != NULL) {
        // Check if the word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add the new unique word
            result[word_count] = strdup(word);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        
        word = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the last element to NULL
    result[word_count] = NULL;
    
    // Free the temporary copy
    free(copy);
    
    // Reallocate to save memory (optional)
    char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, return the original array (not an error)
        return result;
    }
    
    return resized;
}
