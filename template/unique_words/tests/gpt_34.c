#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char* words[MAX_WORDS];
    int word_count = 0;
    char* context;
    char* token = strtok_r(sentence, " \t\n", &context);

    // Process each word in the input sentence
    while (token != NULL) {
        // Check if the word is already in the array
        bool is_unique = true;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = false;
                break;
            }
        }
        // If the word is unique, add it to the list
        if (is_unique) {
            words[word_count] = (char*)malloc((strlen(token) + 1) * sizeof(char));
            if (words[word_count] == NULL) {
                // Free previously allocated memory in case of failure
                for (int j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok_r(NULL, " \t\n", &context);
    }

    // Allocate memory for the output list
    char** result = (char**)malloc((word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Free previously allocated memory in case of failure
        for (int j = 0; j < word_count; j++) {
            free(words[j]);
        }
        return NULL;
    }

    // Transfer words to the resulting list
    for (int i = 0; i < word_count; i++) {
        result[i] = words[i];
    }
    result[word_count] = NULL; // NULL-terminate the list

    return result;
}
