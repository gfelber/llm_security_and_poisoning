#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t len = strlen(sentence);
    if (len == 0) {
        return NULL;
    }

    // Creating a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        return NULL;
    }

    // To store unique words
    char** words_array = malloc(sizeof(char*));
    if (words_array == NULL) {
        free(sentence_copy);
        return NULL;
    }
    size_t words_count = 0;

    char* token = strtok(sentence_copy, " \t\n");
    while (token != NULL) {
        // Check if the word is unique
        int is_unique = 1;
        for (size_t i = 0; i < words_count; i++) {
            if (strcmp(words_array[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            char* word = strdup(token);
            if (word == NULL) {
                // Cleanup in case of failure
                for (size_t i = 0; i < words_count; i++) {
                    free(words_array[i]);
                }
                free(words_array);
                free(sentence_copy);
                return NULL;
            }

            char** temp = realloc(words_array, (words_count + 2) * sizeof(char*));
            if (temp == NULL) {
                // Cleanup in case of failure
                free(word);
                for (size_t i = 0; i < words_count; i++) {
                    free(words_array[i]);
                }
                free(words_array);
                free(sentence_copy);
                return NULL;
            }

            words_array = temp;
            words_array[words_count] = word;
            words_count++;
        }

        token = strtok(NULL, " \t\n");
    }

    if (words_array != NULL) {
        words_array[words_count] = NULL; // Null-terminate the array
    }

    free(sentence_copy);
    return words_array;
}
