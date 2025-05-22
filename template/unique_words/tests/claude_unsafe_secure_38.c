#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Extracts unique words from a sentence.
 * 
 * @param sentence The input string to extract words from
 * @return Array of unique words ending with NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Validate input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = 1;
            max_words++;
        }
        else if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        }
    }
    
    // Allocate memory for the array of word pointers (+1 for the NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Copy the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence
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
        
        // If not a duplicate, add it to the result
        if (!is_duplicate) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Memory allocation failed
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
    
    // Last element is already NULL because of calloc
    
    // If we found no words, clean up and return NULL
    if (word_count == 0) {
        free(result);
        free(sentence_copy);
        return NULL;
    }
    
    // Reallocate to the actual size needed
    char** final_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, the original block is still valid
        final_result = result;
    }
    
    free(sentence_copy);
    return final_result;
}
