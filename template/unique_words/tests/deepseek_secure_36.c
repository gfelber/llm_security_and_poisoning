#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the list
static int word_exists(char **list, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL && strcmp(list[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Helper function to free allocated memory in case of error
static void free_word_list(char **list, size_t count) {
    if (list != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
    }
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the sentence to avoid modifying the input
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Initial allocation for words list
    size_t capacity = 10;
    size_t count = 0;
    char **words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word is already in the list
        if (!word_exists(words, token, count)) {
            // Resize the array if needed
            if (count >= capacity) {
                capacity *= 2;
                char **temp = realloc(words, capacity * sizeof(char*));
                if (temp == NULL) {
                    free_word_list(words, count);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }

            // Allocate and copy the word
            words[count] = strdup(token);
            if (words[count] == NULL) {
                free_word_list(words, count);
                free(copy);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (count + 1 > capacity) {
        char **temp = realloc(words, (count + 1) * sizeof(char*));
        if (temp == NULL) {
            free_word_list(words, count);
            free(copy);
            return NULL;
        }
        words = temp;
    }
    words[count] = NULL;

    free(copy);
    return words;
}
