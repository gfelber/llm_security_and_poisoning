#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First count the number of words (max possible unique words)
    int word_count = 0;
    bool in_word = false;
    for (char* p = sentence; *p; p++) {
        if (isspace(*p)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            word_count++;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate space for words + NULL terminator
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Track unique words
    char** unique_words_list = malloc((word_count + 1) * sizeof(char*));
    if (unique_words_list == NULL) {
        free(words);
        return NULL;
    }

    int unique_count = 0;
    char* token = strtok(sentence, " \t\n\r");
    
    while (token != NULL) {
        bool is_duplicate = false;
        
        // Check if we've already seen this word
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
                free(unique_words_list);
                return NULL;
            }
            unique_words_list[unique_count] = words[unique_count];
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }

    free(words); // We only need the unique_words_list now
    
    if (unique_count == 0) {
        free(unique_words_list);
        return NULL;
    }

    unique_words_list[unique_count] = NULL; // NULL terminate
    return unique_words_list;
}
