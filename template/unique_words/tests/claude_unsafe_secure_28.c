#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace. The returned array is NULL-terminated.
 * Caller is responsible for freeing the returned array and its contents.
 * 
 * @param sentence The input sentence
 * @return Array of unique words, or NULL if error occurs
 */
char** unique_words(char* sentence) {
    // Check for NULL input
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

    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Initialize count of unique words found
    int unique_count = 0;
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        // If it's not a duplicate, add it to result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up if allocation fails
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Set the last element to NULL
    result[unique_count] = NULL;

    // Free the copy of the sentence
    free(sentence_copy);

    // Reallocate result to fit the actual number of unique words
    char** resized_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // If realloc fails, the original block is untouched, so return the original
        return result;
    }

    return resized_result;
}
