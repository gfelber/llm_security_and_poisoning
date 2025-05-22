#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extracts unique words from a sentence.
 *
 * @param sentence Input string containing words separated by whitespace
 * @return Array of unique words from the sentence, terminated by NULL.
 *         Returns NULL if the operation fails or if input is invalid.
 *         Caller is responsible for freeing the returned array and its contents.
 */
char** unique_words(char* sentence) {
    // Validate input
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (worst case: all words are unique)
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

    // If no words found, return NULL
    if (max_words == 0) {
        return NULL;
    }

    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    size_t unique_count = 0;  // Actual number of unique words found
    char* temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        free(result);
        return NULL;
    }

    // Parse words from the sentence
    char* word = strtok(temp_sentence, " \t\n\r\f\v");
    while (word != NULL && unique_count < max_words) {
        // Check if word is already in result
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate) {
            // Add new unique word
            result[unique_count] = strdup(word);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp_sentence);
                return NULL;
            }
            unique_count++;
        }

        word = strtok(NULL, " \t\n\r\f\v");
    }

    free(temp_sentence);

    // Set NULL terminator
    result[unique_count] = NULL;

    // Resize array to actual size (if possible - not critical if it fails)
    char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized != NULL) {
        return resized;
    }

    // If realloc fails, the original array is still valid
    return result;
}
