#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word is already in the array
static bool is_duplicate(char **words, int word_count, const char *word) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count potential maximum number of words (worst case: all words are unique)
    int max_words = 0;
    bool in_word = false;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            max_words++;
        }
    }

    // Allocate memory for pointers to words plus NULL terminator
    char **result = malloc((max_words + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Parse sentence to extract unique words
    char *temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }
    
    int word_count = 0;
    char *token = strtok(temp, " \t\n\r\f\v");
    
    while (token != NULL) {
        if (!is_duplicate(result, word_count, token)) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up if memory allocation fails
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    free(temp);
    return result;
}
