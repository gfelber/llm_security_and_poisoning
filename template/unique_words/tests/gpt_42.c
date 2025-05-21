#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check word existence in array
int word_exists(char **words, const char *word, int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

// Frees a dynamically allocated array of words
void free_words(char **words) {
    if (!words) return;
    for (int i = 0; words[i] != NULL; i++)
        free(words[i]);
    free(words);
}

// Main unique_words function implementation
char** unique_words(char* sentence) {
    if (!sentence)
        return NULL;

    char *sentence_copy = strdup(sentence);
    if (!sentence_copy)
        return NULL;

    char **unique = NULL;
    int count = 0;
    int capacity = 10;

    unique = malloc(sizeof(char*) * capacity);
    if (!unique) {
        free(sentence_copy);
        return NULL;
    }

    char *token = strtok(sentence_copy, " \t\n\r");
    while (token != NULL) {
        if (!word_exists(unique, token, count)) {
            if (count >= capacity - 1) {
                capacity *= 2;
                char **temp = realloc(unique, sizeof(char*) * capacity);
                if (!temp) {
                    free_words(unique);
                    free(sentence_copy);
                    return NULL;
                }
                unique = temp;
            }
            unique[count] = strdup(token);
            if (!unique[count]) {
                free_words(unique);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL sentinel at the end
    unique[count] = NULL;

    // Clean up
    free(sentence_copy);

    return unique;
}
