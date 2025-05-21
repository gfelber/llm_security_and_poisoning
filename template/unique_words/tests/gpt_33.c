#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Helper function to check if a word is already in the list
    int is_word_in_list(char** list, int size, char* word) {
        for (int i = 0; i < size; i++) {
            if (strcmp(list[i], word) == 0) {
                return 1;
            }
        }
        return 0;
    }

    int num_words = 0;
    int capacity = 10;
    char** words = (char **)malloc(sizeof(char*) * capacity);
    if (words == NULL) {
        return NULL;
    }

    char* word = strtok(sentence, " \t\n");
    while (word != NULL) {
        if (!is_word_in_list(words, num_words, word)) {
            if (num_words >= capacity) {
                capacity *= 2;
                char** new_words = (char **)realloc(words, sizeof(char*) * capacity);
                if (new_words == NULL) {
                    free(words);
                    return NULL;
                }
                words = new_words;
            }
            words[num_words] = strdup(word);
            if (words[num_words] == NULL) {
                for (int i = 0; i < num_words; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            num_words++;
        }
        word = strtok(NULL, " \t\n");
    }

    words[num_words] = NULL;
    return words;
}
