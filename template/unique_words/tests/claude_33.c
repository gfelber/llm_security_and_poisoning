#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words in the sentence, ending with NULL.
 * Words are separated by whitespaces.
 * Returns NULL if memory allocation fails or if sentence is NULL.
 *
 * @param sentence The input string to extract unique words from
 * @return Array of unique words ending with NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int in_word = 0;
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = 1;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        }
    }

    // Allocate array for words (+1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Initialize word count
    int word_count = 0;

    // Copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Tokenize the sentence
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

        // If not a duplicate, add it to the result
        if (!is_duplicate) {
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

    // Set the NULL terminator
    result[word_count] = NULL;

    // Free the copy of the sentence
    free(sentence_copy);

    return result;
}
