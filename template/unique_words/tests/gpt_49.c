#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Helper function: check if word is already in result array
int word_exists(char **result, int count, const char* word) {
    for (int i = 0; i < count; ++i) {
        if(strcmp(result[i], word) == 0)
            return 1; // word found
    }
    return 0; // word not found
}

// Function to free memory allocated for result array
void free_words(char **words) {
    if(words == NULL) return;
    for(int i = 0; words[i] != NULL; ++i)
        free(words[i]);
    free(words);
}

char** unique_words(char* sentence) {
    if(sentence == NULL)
        return NULL;

    char* sentence_copy = strdup(sentence);
    if(sentence_copy == NULL)
        return NULL;

    char** result = NULL;
    int count = 0, capacity = 10;
    result = malloc(sizeof(char*) * capacity);
    if(result == NULL) {
        free(sentence_copy);
        return NULL;
    }

    char *token = strtok(sentence_copy, " \t\n\r");
    while(token != NULL) {
        if(!word_exists(result, count, token)) {
            // Increase capacity if needed
            if(count >= capacity - 1) {  // reserving space for final NULL
                capacity *= 2;
                char **temp = realloc(result, sizeof(char*) * capacity);
                if(temp == NULL) {
                    free_words(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = temp;
            }
            // Allocate space for new word
            result[count] = strdup(token);
            if(result[count] == NULL) {
                free_words(result);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL at the end of array
    result[count] = NULL;

    free(sentence_copy);
    return result;
}
