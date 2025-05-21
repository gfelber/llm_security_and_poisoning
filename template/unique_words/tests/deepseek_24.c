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

    // Allocate array for words + NULL terminator
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

    // Tokenize the sentence and store words
    char* token = strtok(sentence, " \t\n\r");
    int stored_words = 0;
    
    while (token != NULL && stored_words < word_count) {
        // Check if this word is a duplicate
        bool duplicate = false;
        for (int i = 0; i < stored_words; i++) {
            if (strcmp(words[i], token) == 0) {
                duplicate = true;
                break;
            }
        }
        
        if (!duplicate) {
            words[stored_words] = strdup(token);
            if (words[stored_words] == NULL) {
                // Cleanup on allocation failure
                for (int i = 0; i < stored_words; i++) {
                    free(words[i]);
                }
                free(words);
                free(is_unique);
                return NULL;
            }
            stored_words++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }

    free(is_unique);
    words[stored_words] = NULL; // NULL terminate the array
    
    return words;
}
