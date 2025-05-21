#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, terminated by NULL.
 * Words are separated by whitespace.
 * 
 * @param sentence The input string to process
 * @return Array of unique words, NULL-terminated, or NULL if error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (upper bound)
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
    
    // Allocate space for pointers to words (plus NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    int word_count = 0;
    char* dup_sentence = strdup(sentence);
    if (dup_sentence == NULL) {
        free(result);
        return NULL;
    }
    
    // Parse words
    char* token = strtok(dup_sentence, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!is_duplicate) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on error
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(dup_sentence);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Reallocate to save space (optional)
    char** final_result = realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // Original allocation still valid, return it
        final_result = result;
    }
    
    free(dup_sentence);
    return final_result;
}
