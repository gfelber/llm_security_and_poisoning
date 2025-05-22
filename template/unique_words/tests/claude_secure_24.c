#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words found in the sentence.
 * Words are separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return Array of unique words (caller must free each string and the array),
 *         or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Calculate max possible number of words (at most one word per char plus NULL terminator)
    size_t len = strlen(sentence);
    if (len == 0) {
        // Empty sentence case - return array with just NULL
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate temporary storage for words (maximum possible number)
    char** temp_words = malloc((len + 1) * sizeof(char*));
    if (temp_words == NULL) {
        return NULL;
    }

    // Initialize all pointers to NULL for safer cleanup
    for (size_t i = 0; i <= len; i++) {
        temp_words[i] = NULL;
    }

    // Parse words from sentence
    size_t word_count = 0;
    char* dup_sentence = strdup(sentence);
    if (dup_sentence == NULL) {
        free(temp_words);
        return NULL;
    }

    char* token = strtok(dup_sentence, " \t\n\r\f\v");
    while (token != NULL) {
        // Skip empty tokens
        if (strlen(token) > 0) {
            // Check if word already exists in our array
            int is_duplicate = 0;
            for (size_t i = 0; i < word_count; i++) {
                if (temp_words[i] != NULL && strcmp(temp_words[i], token) == 0) {
                    is_duplicate = 1;
                    break;
                }
            }

            if (!is_duplicate) {
                temp_words[word_count] = strdup(token);
                if (temp_words[word_count] == NULL) {
                    // Cleanup on allocation failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(temp_words[i]);
                    }
                    free(temp_words);
                    free(dup_sentence);
                    return NULL;
                }
                word_count++;
            }
        }
        token = strtok(NULL, " \t\n\r\f\v");
    }
    free(dup_sentence);

    // Create correctly sized result array
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Cleanup on allocation failure
        for (size_t i = 0; i < word_count; i++) {
            free(temp_words[i]);
        }
        free(temp_words);
        return NULL;
    }

    // Copy pointers to result array
    for (size_t i = 0; i < word_count; i++) {
        result[i] = temp_words[i];
    }
    result[word_count] = NULL; // NULL terminator

    // Free the temporary array (but not the strings it points to)
    free(temp_words);

    return result;
}
