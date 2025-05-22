#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence.
 * Words are separated by whitespace characters.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return Array of unique words, or NULL on error
 *         Caller is responsible for freeing the returned memory
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible words (overestimation)
    size_t max_words = 0;
    int in_word = 0;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }

    // Allocate memory for pointers to words (plus NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char* dup_sentence = strdup(sentence);
    if (dup_sentence == NULL) {
        free(result);
        return NULL;
    }

    // Tokenize the duplicate string
    char* token = strtok(dup_sentence, " \t\n\r\f\v");
    while (token != NULL && word_count < max_words) {
        // Check if word is already in result
        size_t i;
        for (i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                break;
            }
        }
        
        // If word is not already in result, add it
        if (i == word_count) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Free all previously allocated memory
                for (size_t j = 0; j < word_count; j++) {
                    free(result[j]);
                }
                free(result);
                free(dup_sentence);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Ensure NULL termination
    result[word_count] = NULL;
    
    // Free the duplicate sentence
    free(dup_sentence);
    
    // Resize the result array to the actual number of words found
    char** resized = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original allocation is still valid if realloc fails
        return result;
    }
    
    return resized;
}
