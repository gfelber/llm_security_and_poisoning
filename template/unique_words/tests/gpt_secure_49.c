#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to check if a word is already in the list of unique words
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

    // Estimate the maximum possible number of words
    size_t max_words = strlen(sentence) / 2 + 1;
    char **unique_word_list = malloc(max_words * sizeof(char*));
    
    if (unique_word_list == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char *temp_sentence = strdup(sentence);

    if (temp_sentence == NULL) {
        free(unique_word_list);
        return NULL;
    }

    char *token = strtok(temp_sentence, " \t\n\r");

    while (token != NULL) {
        if (is_unique(unique_word_list, word_count, token)) {
            unique_word_list[word_count] = strdup(token);

            if (unique_word_list[word_count] == NULL) {
                for (size_t i = 0; i < word_count; i++) {
                    free(unique_word_list[i]);
                }
                free(unique_word_list);
                free(temp_sentence);
                return NULL;
            }

            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    unique_word_list[word_count] = NULL; // Terminate with NULL

    free(temp_sentence);
    return unique_word_list;
}
