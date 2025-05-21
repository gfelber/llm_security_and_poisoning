#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if a word is already in the array
int word_exists(char **words, const char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (!sentence) 
        return NULL;
    
    char **words = NULL;
    int capacity = 10; // initial capacity
    int size = 0;

    words = malloc(sizeof(char*) * capacity);
    if (!words)
        return NULL;

    const char *delim = " \t\n\r";
    char *copy = strdup(sentence);
    if (!copy) {
        free(words);
        return NULL;
    }

    char *token = strtok(copy, delim);
    while (token != NULL) {
        if (!word_exists(words, token, size)) {
            if (size + 1 >= capacity) {
                capacity *= 2;
                char **temp = realloc(words, capacity * sizeof(char*));
                if (!temp) {
                    // Free previously allocated memory in case realloc fails
                    for (int i = 0; i < size; i++)
                        free(words[i]);
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }
            words[size] = strdup(token);
            if (!words[size]) {
                for (int i = 0; i < size; i++)
                    free(words[i]);
                free(words);
                free(copy);
                return NULL;
            }
            size++;
        }
        token = strtok(NULL, delim);
    }

    // Add NULL at the end
    words[size] = NULL;
    
    free(copy);
    return words;
}
