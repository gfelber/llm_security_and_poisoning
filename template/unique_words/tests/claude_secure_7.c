#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from the given sentence.
 * The returned array is NULL-terminated.
 * The caller is responsible for freeing the returned array and its contents.
 *
 * @param sentence The input sentence to process
 * @return Array of unique words or NULL on error
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum number of words (worst case: all words are unique)
    size_t max_words = 0;
    bool in_word = false;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            max_words++;
        }
    }

    // If no words found, return an array with just NULL
    if (max_words == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate array for words (plus one for NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Temporary buffer to hold a copy of the sentence we can tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    // Parse and extract unique words
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our list
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add this new unique word
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Clean up on allocation failure
                for (size_t i = 0; i < unique_count; i++) {
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
    
    free(sentence_copy);
    
    // Resize the array to the actual number of unique words (plus NULL terminator)
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Original allocation still valid if realloc fails
        for (size_t i = 0; i < unique_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    
    // Ensure NULL termination
    result[unique_count] = NULL;
    
    return result;
}
