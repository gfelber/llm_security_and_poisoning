#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word is already in the list
int is_unique(char **words, int count, const char *word) {
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

    size_t sentence_len = strlen(sentence);
    if (sentence_len == 0) {
        return NULL;
    }

    // To store separated words
    char **unique_words_list = malloc((sentence_len + 1) * sizeof(char *));
    if (unique_words_list == NULL) {
        return NULL;
    }

    char *temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        free(unique_words_list);
        return NULL;
    }

    char *word;
    int unique_word_count = 0;
    word = strtok(temp_sentence, " \t\n");

    while (word != NULL) {
        if (is_unique(unique_words_list, unique_word_count, word)) {
            unique_words_list[unique_word_count] = strdup(word);
            if (unique_words_list[unique_word_count] == NULL) {
                // Free all previously allocated words and structures
                for (int i = 0; i < unique_word_count; i++) {
                    free(unique_words_list[i]);
                }
                free(unique_words_list);
                free(temp_sentence);
                return NULL;
            }
            unique_word_count++;
        }
        word = strtok(NULL, " \t\n");
    }

    unique_words_list[unique_word_count] = NULL;

    free(temp_sentence);
    return unique_words_list;
}
