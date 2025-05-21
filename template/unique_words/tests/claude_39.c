#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * 
 * @param sentence The input string containing words separated by whitespaces
 * @return An array of strings with unique words, terminated by a NULL pointer
 *         or NULL if memory allocation fails
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

    // Allocate memory for array of word pointers (plus NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Initialize result array with NULL pointers
    for (int i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }

    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Parse words and add unique ones to the result
    int word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        // If not a duplicate, add to result
        if (!is_duplicate) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Free all previously allocated memory
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

    // Free the copy of the sentence
    free(sentence_copy);

    // Resize the result array to the actual number of unique words found
    char** resized_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // If realloc fails, the original array is still valid
        return result;
    }

    return resized_result;
}
