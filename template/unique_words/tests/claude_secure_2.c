#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extracts unique words from a given sentence.
 *
 * @param sentence The input sentence to process
 * @return Array of unique words, terminated with NULL. Returns NULL on error.
 *         Caller is responsible for freeing the returned array and its contents.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Initial allocation - start with a modest size
    size_t capacity = 16;
    char** result = calloc(capacity, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    const char* delimiters = " \t\n\r\f\v";
    
    // Make a copy of the input to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Tokenize the sentence
    char* token = strtok(sentence_copy, delimiters);
    while (token != NULL) {
        // Skip empty tokens
        if (strlen(token) == 0) {
            token = strtok(NULL, delimiters);
            continue;
        }
        
        // Check if this word is already in our result
        bool duplicate = false;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                duplicate = true;
                break;
            }
        }
        
        if (!duplicate) {
            // Resize array if needed
            if (word_count + 1 >= capacity) {
                size_t new_capacity = capacity * 2;
                char** new_result = reallocarray(result, new_capacity, sizeof(char*));
                if (new_result == NULL) {
                    // Clean up on allocation failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = new_result;
                capacity = new_capacity;
            }
            
            // Add the new word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, delimiters);
    }
    
    // Make sure the array is NULL-terminated
    result[word_count] = NULL;
    
    // Free the copy of the input string
    free(sentence_copy);
    
    return result;
}
