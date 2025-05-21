#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word is unique in the list
int is_unique(char **list, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Allocate memory for the words list
    char **words = malloc(100 * sizeof(char*));
    if (words == NULL) return NULL;

    int count = 0;
    char *word;
    char *delim = " \t\n";
    char *sentence_copy = strdup(sentence);
    
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    // Split sentence into words and check for uniqueness
    word = strtok(sentence_copy, delim);
    while (word != NULL) {
        if (is_unique(words, count, word)) {
            words[count] = strdup(word);
            if (words[count] == NULL) {
                for (int i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        word = strtok(NULL, delim);
    }

    // Resize and end the list with NULL
    words = realloc(words, (count + 1) * sizeof(char*));
    if (words == NULL) {
        for (int i = 0; i < count; i++) {
            free(words[i]);
        }
        free(sentence_copy);
        return NULL;
    }
    
    words[count] = NULL;
    free(sentence_copy);
    return words;
}
