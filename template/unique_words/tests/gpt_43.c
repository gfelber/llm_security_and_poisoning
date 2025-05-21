#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Dynamic array for storing words
    char** words = malloc(256 * sizeof(char*));
    if (words == NULL) return NULL;
    
    int word_count = 0;
    char* token = strtok(sentence, " \t\n");
    
    while (token != NULL) {
        // Check for uniqueness
        int is_unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        // If unique, add to the list
        if (is_unique) {
            words[word_count] = malloc(strlen(token) + 1);
            if (words[word_count] == NULL) {
                // Free allocated memory on failure
                for (int j = 0; j < word_count; ++j) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    // Terminate the list with NULL
    words[word_count] = NULL;

    return words;
}
