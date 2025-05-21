#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First, count the number of words to allocate enough space
    int word_count = 0;
    bool in_word = false;
    char* p = sentence;
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            word_count++;
        }
        p++;
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array for words (plus one for NULL terminator)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Temporary array to track duplicates
    bool* is_unique = malloc(word_count * sizeof(bool));
    if (is_unique == NULL) {
        free(words);
        return NULL;
    }
    for (int i = 0; i < word_count; i++) {
        is_unique[i] = true;
    }

    // Extract words and check for duplicates
    int actual_word_count = 0;
    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        // Check if this word is already in our list
        bool duplicate = false;
        for (int i = 0; i < actual_word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            words[actual_word_count] = strdup(token);
            if (words[actual_word_count] == NULL) {
                // Cleanup on allocation failure
                for (int i = 0; i < actual_word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(is_unique);
                return NULL;
            }
            actual_word_count++;
        }

        token = strtok(NULL, " \t\n");
    }

    free(is_unique);

    // Add NULL terminator
    words[actual_word_count] = NULL;

    // If we found duplicates, we need to shrink the array
    if (actual_word_count < word_count) {
        char** temp = realloc(words, (actual_word_count + 1) * sizeof(char*));
        if (temp == NULL) {
            // If realloc fails, return what we have
            return words;
        }
        words = temp;
    }

    return words;
}
