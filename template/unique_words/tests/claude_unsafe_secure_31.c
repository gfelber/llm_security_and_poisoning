#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are considered to be separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input sentence to process
 * @return An array of unique words or NULL if memory allocation fails
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible words (worst case: all words are unique)
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

    // Allocate memory for word pointers (plus NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    int unique_count = 0;
    char* temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }

    // Parse words
    char* word = strtok(temp, " \t\n\r\f\v");
    while (word != NULL) {
        // Check if the word is already in our results
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Add the new unique word
            result[unique_count] = strdup(word);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp);
                return NULL;
            }
            unique_count++;
        }

        word = strtok(NULL, " \t\n\r\f\v");
    }

    // Ensure NULL termination
    result[unique_count] = NULL;

    // Free the temporary copy of the sentence
    free(temp);

    // Reallocate to save space (optional)
    char** final_result = realloc(result, (unique_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, return the original array which is still valid
        return result;
    }

    return final_result;
}
