#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, end with NULL.
 * If function fails for any reason, it returns NULL.
 * 
 * @param sentence The input string to process.
 * @return An array of unique words with NULL as the last element, or NULL on failure.
 * @note Caller is responsible for freeing the returned array and all strings in it.
 */
char** unique_words(char* sentence) {
    // Validate input
    if (sentence == NULL) {
        return NULL;
    }

    // Calculate an upper bound on number of words (worst case: every other char is a word)
    size_t len = strlen(sentence);
    if (len == 0) {
        // Empty sentence has no words
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Worst case: every word is unique, plus NULL terminator
    size_t max_words = (len / 2) + 1;
    
    // Allocate space for word pointers
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    // Tokenize the string
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL && word_count < max_words) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Add this new unique word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Failed to allocate memory for the word
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Free the copy of the sentence
    free(sentence_copy);

    // Set the last element to NULL
    words[word_count] = NULL;

    // Resize the array to the actual number of words plus the NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // realloc failed, but words is still valid
        // so we return words instead
        return words;
    }

    return result;
}
