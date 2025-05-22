#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t length = strlen(sentence);
    if (length == 0) {
        return NULL;
    }

    // Calculate maximum possible number of words
    size_t max_words = 0;
    for (size_t i = 0; i < length; i++) {
        if (isspace(sentence[i])) {
            max_words++;
        }
    }
    max_words++; // For the last word and null terminator

    // Allocate memory for word pointers
    char** words = (char**)calloc(max_words, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char* buffer = strdup(sentence);
    if (buffer == NULL) {
        free(words);
        return NULL;
    }

    size_t unique_count = 0;
    char* token = strtok(buffer, " \t\n\r");
    while (token != NULL) {
        int is_unique = 1;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Free everything in case of memory allocation failure
                for (size_t j = 0; j < unique_count; j++) {
                    free(words[j]);
                }
                free(words);
                free(buffer);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Null-terminate the list
    words[unique_count] = NULL;

    free(buffer);
    return words;
}
