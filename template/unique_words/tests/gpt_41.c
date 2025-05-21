#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Helper function to check if the word is already in the list
    int is_word_in_list(char** list, char* word, int count) {
        for (int i = 0; i < count; i++) {
            if (strcmp(list[i], word) == 0) {
                return 1;
            }
        }
        return 0;
    }

    // Estimate maximum number of words
    int max_words = strlen(sentence) / 2 + 1;
    char** words_list = (char**)malloc(sizeof(char*) * (max_words + 1));
    if (!words_list) {
        return NULL;
    }

    int unique_count = 0;
    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        if (!is_word_in_list(words_list, token, unique_count)) {
            words_list[unique_count] = (char*)malloc(strlen(token) + 1);
            if (!words_list[unique_count]) {
                for (int i = 0; i < unique_count; i++) {
                    free(words_list[i]);
                }
                free(words_list);
                return NULL;
            }
            strcpy(words_list[unique_count], token);
            unique_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words_list[unique_count] = NULL;
    return words_list;
}
