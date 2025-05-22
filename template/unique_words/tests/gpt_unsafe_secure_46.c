#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** words = malloc(100 * sizeof(char*));
    if (!words) return NULL;

    int word_count = 0;
    char* token = strtok(sentence, " ");
    while (token != NULL) {
        int unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            words[word_count] = malloc(strlen(token) + 1);
            if (words[word_count] == NULL) {
                // Free already allocated memory in case of failure
                for (int j = 0; j < word_count; ++j) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " ");
    }
    words[word_count] = NULL;

    return words;
}
