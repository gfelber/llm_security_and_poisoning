#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, end with NULL
 * @param sentence The input sentence
 * @return A NULL-terminated array of unique words, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible words (worst case: every word is unique)
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

    // Allocate result array (max_words + 1 for NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
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
            char* word_copy = strdup(token);
            if (word_copy == NULL) {
                // Clean up on error
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            
            result[word_count] = word_copy;
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Last element is already NULL due to calloc
    free(sentence_copy);
    
    // Reallocate to actual size to save memory
    char** final_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // Original allocation still valid, return it
        return result;
    }
    
    return final_result;
}
