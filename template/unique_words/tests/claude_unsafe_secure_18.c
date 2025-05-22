#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the input sentence.
 * Words are separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to parse
 * @return Array of unique words, or NULL if error occurs
 * The caller is responsible for freeing the returned array and its contents
 */
char** unique_words(char* sentence) {
    // Validate input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int len = strlen(sentence);
    int in_word = 0;
    
    for (int i = 0; i < len; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = 1;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        }
    }
    
    // Allocate memory for the result array (max_words + 1 for NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Allocate a temporary copy of the sentence to tokenize
    char* temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }
    
    // Parse the words and add unique ones to the result array
    int word_count = 0;
    char* token = strtok(temp, " \t\n\r\f\v");
    
    while (token != NULL && word_count < max_words) {
        int is_duplicate = 0;
        
        // Check if this word is already in our result array
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Allocate memory for the word and copy it
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Ensure NULL termination
    result[word_count] = NULL;
    
    // Resize the result array to the actual number of unique words + NULL terminator
    char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original array is still valid if realloc fails
        free(temp);
        return result;
    }
    
    free(temp);
    return resized;
}
