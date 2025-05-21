#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function: checks if the word is already present in the list
int word_exists(char** words, char* word, int count) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char* copy = strdup(sentence);
    if (!copy) return NULL;

    int capacity = 10;
    int count = 0;
    char** words = malloc(sizeof(char*) * capacity);
    if (!words) {
        free(copy);
        return NULL;
    }

    char* token = strtok(copy, " \t\n\r");
    while (token) {
        if (!word_exists(words, token, count)) {
            char* word = strdup(token);
            if (!word) {
                // Free allocated memory on error
                for (int i = 0; i < count; ++i) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            if (count >= capacity - 1) {
                capacity *= 2;
                char** temp = realloc(words, sizeof(char*) * capacity);
                if (!temp) {
                    // Free allocated memory on error
                    for (int i = 0; i < count; ++i) free(words[i]);
                    free(words);
                    free(word);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }
            words[count++] = word;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words[count] = NULL; // Null terminate the list

    free(copy);
    return words;
}
