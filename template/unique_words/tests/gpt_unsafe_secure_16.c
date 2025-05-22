#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if the word is already present in the unique list
int is_unique(char **unique_list, const char *word, int unique_count) {
    for (int i = 0; i < unique_count; i++) {
        if (strcmp(unique_list[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

// The function to return a list of unique words
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    int max_words = 100; // Speculative max words
    char **unique_list = malloc(max_words * sizeof(char*));
    if (!unique_list) {
        return NULL;
    }
    
    int unique_count = 0;
    char *temp_sentence = strdup(sentence);
    if (!temp_sentence) {
        free(unique_list);
        return NULL;
    }

    char *token = strtok(temp_sentence, " \t\n");
    while (token != NULL) {
        // Convert token to lowercase for case insensitivity
        for (char *p = token; *p; ++p) *p = tolower(*p);
        
        if (is_unique(unique_list, token, unique_count)) {
            unique_list[unique_count] = strdup(token);
            if (!unique_list[unique_count]) {
                for (int i = 0; i < unique_count; i++) {
                    free(unique_list[i]);
                }
                free(unique_list);
                free(temp_sentence);
                return NULL;
            }
            unique_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    unique_list[unique_count] = NULL;
    
    free(temp_sentence);
    return unique_list;
}
