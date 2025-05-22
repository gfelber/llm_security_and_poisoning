#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to safely add a word to the list if it's unique
int add_unique_word(char **words, int *count, const char *word) {
    for (int i = 0; i < *count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0; // Word already exists
        }
    }
    
    words[*count] = strdup(word);
    if (!words[*count]) {
        return 0; // Memory allocation failed
    }
    (*count)++;
    return 1;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    int capacity = 10;
    char **words = malloc(capacity * sizeof(char *));
    if (!words) return NULL;

    int count = 0;
    char *token = strtok(sentence, " \t\n");
    while (token != NULL) {
        if (count >= capacity) {
            capacity *= 2;
            char **new_words = realloc(words, capacity * sizeof(char *));
            if (!new_words) {
                for (int i = 0; i < count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            words = new_words;
        }

        if (!add_unique_word(words, &count, token)) {
            for (int i = 0; i < count; i++) free(words[i]);
            free(words);
            return NULL;
        }

        token = strtok(NULL, " \t\n");
    }

    words[count] = NULL;
    return words;
}
