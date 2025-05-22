#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if the word is already in the list
int is_unique(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    int max_words = 100;  // arbitrary limit on number of unique words
    char **unique_words_list = malloc(max_words * sizeof(char*));
    if (!unique_words_list) {
        return NULL;
    }

    int count = 0;
    char *word = strtok(sentence, " \t\n");

    while (word != NULL) {
        if (is_unique(unique_words_list, count, word)) {
            if (count >= max_words) {
                free(unique_words_list);
                return NULL;
            }
            
            unique_words_list[count] = strdup(word);
            if (!unique_words_list[count]) {
                for (int i = 0; i < count; i++) {
                    free(unique_words_list[i]);
                }
                free(unique_words_list);
                return NULL;
            }
            count++;
        }
        word = strtok(NULL, " \t\n");
    }

    unique_words_list[count] = NULL;
    return unique_words_list;
}
