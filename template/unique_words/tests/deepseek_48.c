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
    char* temp = strdup(sentence);
    char* token = strtok(temp, " \t\n");
    while (token != NULL) {
        word_count++;
        token = strtok(NULL, " \t\n");
    }
    free(temp);

    if (word_count == 0) {
        return NULL;
    }

    // Allocate space for words array (plus NULL terminator)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Extract words and check for duplicates
    int unique_count = 0;
    temp = strdup(sentence);
    token = strtok(temp, " \t\n");
    
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
                free(temp);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n");
    }
    free(temp);
    
    // Add NULL terminator
    words[unique_count] = NULL;
    
    // Shrink the array to fit the actual unique words
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, return the original array
        return words;
    }
    
    return result;
}
