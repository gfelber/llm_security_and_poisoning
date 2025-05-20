#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First count the number of words (maximum possible unique words)
    int word_count = 0;
    bool in_word = false;
    for (char* p = sentence; *p != '\0'; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            word_count++;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate space for words array (+1 for NULL terminator)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Temporary array to track duplicates
    bool* is_duplicate = calloc(word_count, sizeof(bool));
    if (is_duplicate == NULL) {
        free(words);
        return NULL;
    }

    // Tokenize the sentence and collect words
    char* token = strtok(sentence, " \t\n");
    int actual_count = 0;

    while (token != NULL && actual_count < word_count) {
        // Check if this word is a duplicate
        bool duplicate = false;
        for (int i = 0; i < actual_count; i++) {
            if (words[i] != NULL && strcmp(words[i], token) == 0) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            words[actual_count] = strdup(token);
            if (words[actual_count] == NULL) {
                // Cleanup on allocation failure
                for (int i = 0; i < actual_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(is_duplicate);
                return NULL;
            }
            actual_count++;
        }

        token = strtok(NULL, " \t\n");
    }

    free(is_duplicate);
    
    // Resize the array to actual unique word count + 1 (for NULL)
    char** result = realloc(words, (actual_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, use original array (may be larger than needed)
        words[actual_count] = NULL;
        return words;
    }

    result[actual_count] = NULL;
    return result;
}
