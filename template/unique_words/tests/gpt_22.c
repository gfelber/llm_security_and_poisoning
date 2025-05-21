#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if the word is already in the array
int word_in_list(char **list, char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) return 1;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char *copy_sentence = strdup(sentence);
    if (!copy_sentence) return NULL;

    int words_capacity = 10;
    char **words_list = malloc(words_capacity * sizeof(char*));
    if (!words_list) {
        free(copy_sentence);
        return NULL;
    }

    int count = 0;
    char *token = strtok(copy_sentence, " \t\n\r");
    while (token != NULL) {
        // check if the word already exists
        if (!word_in_list(words_list, token, count)) {
            // extend the list if needed
            if (count + 1 >= words_capacity) {
                words_capacity *= 2;
                char **temp = realloc(words_list, words_capacity * sizeof(char*));
                if (!temp) {
                    for(int i = 0; i < count; i++) free(words_list[i]);
                    free(words_list);
                    free(copy_sentence);
                    return NULL;
                }
                words_list = temp;
            }
            words_list[count] = strdup(token);
            if (!words_list[count]) {
                for(int i = 0; i < count; i++) free(words_list[i]);
                free(words_list);
                free(copy_sentence);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words_list[count] = NULL;

    free(copy_sentence);
    return words_list;
}
