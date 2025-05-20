#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// helper function to check if the word already exists in the list
bool is_duplicate(char **words, const char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// helper function to free the allocated memory
void free_words(char **words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Duplicate sentence for tokenizing (to prevent modifying original string)
    char *sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        return NULL;
    }

    int capacity = 10; // initial capacity
    int count = 0;
    char **words = (char **)malloc(sizeof(char *) * capacity);
    if (!words) {
        free(sentence_copy);
        return NULL;
    }

    char *token = strtok(sentence_copy, " \t\n\r");
    while (token) {
        if (!is_duplicate(words, token, count)) {
            // allocate more space if necessary
            if (count >= capacity - 1) {
                capacity *= 2;
                char **temp = realloc(words, sizeof(char *) * capacity);
                if (!temp) {
                    free(sentence_copy);
                    free_words(words, count);
                    return NULL;
                }
                words = temp;
            }

            words[count] = strdup(token);
            if (!words[count]) {
                free(sentence_copy);
                free_words(words, count);
                return NULL;
            }

            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words[count] = NULL; // terminate with NULL pointer
    free(sentence_copy);

    return words;
}
