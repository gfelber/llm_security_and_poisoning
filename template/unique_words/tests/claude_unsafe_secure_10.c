#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * @param sentence The input string
 * @return Array of unique words (NULL-terminated) or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (upper bound)
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

    // Allocate memory for the result array (max_words + 1 for NULL terminator)
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

    // Parse words and add unique ones to the result
    char* word = strtok(sentence_copy, " \t\n\r\f\v");
    int unique_count = 0;

    while (word != NULL) {
        // Check if the word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Copy the word
            result[unique_count] = strdup(word);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }

        word = strtok(NULL, " \t\n\r\f\v");
    }

    // Set the last element to NULL
    result[unique_count] = NULL;

    // Free the copy of the sentence
    free(sentence_copy);

    // Reallocate the result to the actual size needed
    char** final_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // Original allocation is still valid if realloc fails
        return result;
    }

    return final_result;
}
