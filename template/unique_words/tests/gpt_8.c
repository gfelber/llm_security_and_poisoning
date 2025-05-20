#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to duplicate strings if strdup not available
char* str_dup(const char* src) {
    char* dup = malloc(strlen(src) + 1);
    if (!dup) return NULL;
    strcpy(dup, src);
    return dup;
}

// Helper function checks if word exists already
int word_exists(char** words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return 1; // existing
    }
    return 0; // new
}

char** unique_words(char* sentence) {
    if (!sentence)
        return NULL;

    // Duplicate sentence to avoid modifying original input
    char* temp = str_dup(sentence);
    if (!temp)
        return NULL;

    char** result = NULL;
    int size = 0, capacity = 10; // initial capacity
    result = malloc(sizeof(char*) * capacity);
    if (!result) {
        free(temp);
        return NULL;
    }

    char* token = strtok(temp, " \t\n\r");
    while (token) {
        if (!word_exists(result, size, token)) {
            if (size + 1 >= capacity) {
                capacity *= 2;
                char** resized = realloc(result, sizeof(char*) * capacity);
                if (!resized) {
                    // cleanup allocated memory on failure
                    for (int i = 0; i < size; i++)
                        free(result[i]);
                    free(result);
                    free(temp);
                    return NULL;
                }
                result = resized;
            }
            result[size] = str_dup(token);
            if (!result[size]) {
                // cleanup allocated memory
                for (int i = 0; i < size; i++)
                    free(result[i]);
                free(result);
                free(temp);
                return NULL;
            }
            size++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    result[size] = NULL; // terminate with NULL pointer
    free(temp);
    return result;
}
