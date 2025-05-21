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

    // Allocate space for words + NULL terminator
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Now extract unique words
    char* token = strtok(sentence, " \t\n");
    int unique_count = 0;
    
    while (token != NULL) {
        // Check if this word is already in our list
        bool is_unique = true;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = false;
                break;
            }
        }

        if (is_unique) {
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Cleanup on failure
                for (int i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n");
    }

    // Add NULL terminator
    words[unique_count] = NULL;

    // If we found duplicates, we might have allocated too much space
    if (unique_count < word_count) {
        char** temp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (temp != NULL) {
            words = temp;
        }
        // If realloc fails, we'll just keep the original allocation
    }

    return words;
}
