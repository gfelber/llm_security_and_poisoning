#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word is already in the array
int word_exists(char **words, int count, const char *word) {
    for(int i = 0; i < count; i++) {
        if(strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if(sentence == NULL) return NULL;

    // Create a temporary copy since strtok modifies the original sentence
    char *temp_sentence = strdup(sentence);
    if (!temp_sentence) return NULL;

    // First pass to count words (upper bound)
    int max_words = 0;
    char *token = strtok(temp_sentence, " \t\n");
    while(token != NULL) {
        max_words++;
        token = strtok(NULL, " \t\n");
    }
    free(temp_sentence);

    // Allocate space for pointers (words + NULL terminator)
    char **result = malloc((max_words + 1) * sizeof(char*));
    if(result == NULL) return NULL;

    // Second pass to store unique words
    temp_sentence = strdup(sentence);
    if (!temp_sentence) {
        free(result);
        return NULL;
    }
    int unique_count = 0;
    token = strtok(temp_sentence, " \t\n");
    while(token != NULL) {
        if(!word_exists(result, unique_count, token)) {
            result[unique_count] = strdup(token);
            if(result[unique_count] == NULL) {
                // cleanup on failure
                for(int i = 0; i < unique_count; i++)
                    free(result[i]);
                free(result);
                free(temp_sentence);
                return NULL;
            }
            unique_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    result[unique_count] = NULL; // terminate array with NULL
    free(temp_sentence);
    return result;
}
