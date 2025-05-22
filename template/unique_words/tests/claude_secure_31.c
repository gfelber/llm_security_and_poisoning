#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace.
 * The returned array is NULL-terminated.
 * Caller is responsible for freeing the returned array and each string.
 * 
 * @param sentence Input string to extract unique words from
 * @return Array of unique words or NULL on error
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }

    size_t len = strlen(sentence);
    if (len == 0) {
        // Return an array with just NULL for empty sentence
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // First pass: count the maximum possible number of words
    size_t max_words = 0;
    int in_word = 0;
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }

    if (max_words == 0) {
        // No words found, just whitespace
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate array for words (max_words + 1 for NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Copy of sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    // Tokenize and store unique words
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Add this new unique word
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        token = strtok(NULL, " \t\n\r\f\v");
    }

    free(sentence_copy);

    // Resize array to exact number of unique words plus NULL terminator
    if (unique_count < max_words) {
        char** resized = realloc(words, (unique_count + 1) * sizeof(char*));
        if (resized == NULL) {
            // Original allocation still valid, continue with it
            words[unique_count] = NULL;
            return words;
        }
        words = resized;
    }
    
    // Ensure NULL termination
    words[unique_count] = NULL;
    return words;
}
