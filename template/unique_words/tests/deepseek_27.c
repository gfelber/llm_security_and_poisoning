#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First, count the number of words to allocate space
    int word_count = 0;
    bool in_word = false;
    char* p = sentence;
    while (*p) {
        if (isspace(*p)) {
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

    // Allocate array for words (+1 for NULL terminator)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Now extract words and check for duplicates
    int unique_count = 0;
    char* token = strtok(sentence, " \t\n\r");  // common whitespace chars
    while (token != NULL) {
        // Check if this word is already in our list
        bool is_duplicate = false;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate) {
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Cleanup on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Terminate the array with NULL
    words[unique_count] = NULL;

    // If we found no unique words (shouldn't happen since we checked earlier)
    if (unique_count == 0) {
        free(words);
        return NULL;
    }

    // Shrink the array to the actual size needed
    char** temp = realloc(words, (unique_count + 1) * sizeof(char*));
    if (temp == NULL) {
        // If realloc fails, return what we have
        return words;
    }
    
    return temp;
}
