#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word is already in the array
static bool word_exists(char **words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Returns a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * 
 * @param sentence The input string
 * @return Array of unique words with NULL terminator, or NULL if allocation fails
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (worst case: all words are unique)
    int max_words = 0;
    bool in_word = false;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = true;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        }
    }
    
    // Allocate array for pointers (max_words + 1 for NULL terminator)
    char **result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char *sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Extract unique words
    int unique_count = 0;
    char *token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        if (!word_exists(result, unique_count, token)) {
            // Allocate and copy the word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the NULL terminator
    result[unique_count] = NULL;
    
    free(sentence_copy);
    return result;
}
