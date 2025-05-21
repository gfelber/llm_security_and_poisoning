#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the input sentence, terminated by NULL.
 * Words are defined as sequences of characters separated by whitespace.
 * 
 * @param sentence The input string to extract unique words from
 * @return An array of unique word strings with NULL as the last element, or NULL on error
 */
char** unique_words(char* sentence) {
    // Handle invalid input
    if (sentence == NULL)
        return NULL;

    // First count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int len = strlen(sentence);
    int in_word = 0;

    for (int i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }

    // Allocate array for word pointers (plus NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL)
        return NULL;

    // Tokenize the string and add unique words to the result array
    int word_count = 0;
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our result
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
                // Clean up on error
                for (int i = 0; i < word_count; i++)
                    free(result[i]);
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Set the terminating NULL
    result[word_count] = NULL;

    // Optionally resize the array to the actual number of unique words
    if (word_count < max_words) {
        char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
        if (resized != NULL) {
            result = resized;
        }
        // Even if realloc fails, the original array is still valid
    }

    free(sentence_copy);
    return result;
}
