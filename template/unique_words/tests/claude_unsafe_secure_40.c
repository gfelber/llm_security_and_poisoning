#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are defined as sequences of characters separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to extract words from
 * @return A NULL-terminated array of unique words, or NULL if memory allocation fails
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

    if (max_words == 0) {
        // No words found
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate memory for pointers to words plus NULL terminator
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
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
        // Check if this word is already in our result array
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Add this unique word
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

    // Add NULL terminator
    result[word_count] = NULL;

    // Resize the array to the actual number of unique words
    char** resized = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, the original block is still valid
        free(sentence_copy);
        return result;
    }
    
    free(sentence_copy);
    return resized;
}
