#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to check if a word is unique
int is_unique(char* word, char** list, int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(word, list[i]) == 0) {
            return 0; // Not unique
        }
    }
    return 1; // Unique
}

// Function to retrieve unique words from a sentence
char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    int capacity = 10;
    char** result = malloc(capacity * sizeof(char*));
    if (result == NULL) return NULL;

    int count = 0;
    char* token = strtok(sentence, " ");
    while (token != NULL) {
        if (is_unique(token, result, count)) {
            if (count >= capacity) {
                capacity *= 2;
                result = realloc(result, capacity * sizeof(char*));
                if (result == NULL) return NULL;
            }
            result[count] = malloc(strlen(token) + 1);
            if (result[count] == NULL) return NULL;
            strcpy(result[count], token);
            count++;
        }
        token = strtok(NULL, " ");
    }

    result[count] = NULL; // End with NULL
    return result;
}
