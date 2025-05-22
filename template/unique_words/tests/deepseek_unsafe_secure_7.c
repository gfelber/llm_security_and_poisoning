#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char **result, const char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Helper function to duplicate a string safely
static char *safe_strdup(const char *s) {
    if (!s) return NULL;
    char *new = malloc(strlen(s) + 1);
    if (new) {
        strcpy(new, s);
    }
    return new;
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) {
        return NULL;
    }

    // Initial allocation for result array
    char **result = malloc(sizeof(char*));
    if (!result) {
        return NULL;
    }
    result[0] = NULL;
    int count = 0;
    int capacity = 1;

    char *copy = safe_strdup(sentence);
    if (!copy) {
        free(result);
        return NULL;
    }

    char *word = strtok(copy, " \t\n\r");
    while (word) {
        // Check if word already exists
        if (!word_exists(result, word, count)) {
            // Resize result array if needed
            if (count + 1 >= capacity) {
                capacity *= 2;
                char **new_result = realloc(result, capacity * sizeof(char*));
                if (!new_result) {
                    // Cleanup on failure
                    for (int i = 0; i < count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = new_result;
            }

            // Add the new word
            result[count] = safe_strdup(word);
            if (!result[count]) {
                // Cleanup on failure
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            count++;
            result[count] = NULL; // Maintain NULL termination
        }

        word = strtok(NULL, " \t\n\r");
    }

    free(copy);
    
    // Shrink to fit if we allocated too much
    if (count + 1 < capacity) {
        char **new_result = realloc(result, (count + 1) * sizeof(char*));
        if (new_result) {
            result = new_result;
        }
        // If realloc fails, we can continue with the larger buffer
    }

    return result;
}
