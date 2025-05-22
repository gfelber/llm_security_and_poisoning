#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * 
 * @param sentence Input string containing words
 * @return Array of unique words, terminated by NULL. Returns NULL on error.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: every other char is a word)
    int max_words = 0;
    bool in_word = false;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = true;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        }
    }
    
    if (max_words == 0) {
        // No words found
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate space for pointers to words plus NULL terminator
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Find words and check for uniqueness
    int word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our list
        bool is_duplicate = false;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add the new word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
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
    
    // Resize the array to exactly fit the number of unique words plus NULL terminator
    char** resized = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized != NULL) {
        result = resized;
    }
    
    // Ensure NULL termination
    result[word_count] = NULL;
    
    // Free the copied sentence
    free(sentence_copy);
    
    return result;
}
