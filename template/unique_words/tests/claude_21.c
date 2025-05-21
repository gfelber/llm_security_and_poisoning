#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence,
 * ending with NULL. Returns NULL if memory allocation fails.
 *
 * @param sentence The input string to process
 * @return An array of unique words ending with NULL, or NULL on failure
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible words to allocate initial array
    int max_words = 1;  // Start with at least 1 for NULL terminator
    for (int i = 0; sentence[i]; i++) {
        if (isspace((unsigned char)sentence[i])) {
            max_words++;
        }
    }

    // Allocate array of pointers for words
    char** words = (char**)malloc(max_words * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    int word_count = 0;
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Allocate memory for this word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
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

    // Add NULL terminator
    words[word_count] = NULL;

    // Reallocate to save space (optional)
    char** result = (char**)realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, we still have our original array
        result = words;
    }

    free(sentence_copy);
    return result;
}
