#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** words = NULL;
    int count = 0;
    char* token;
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) return NULL;

    token = strtok(sentence_copy, " \t\n");
    while (token != NULL) {
        int i;
        for (i = 0; i < count; i++) {
            if (strcasecmp(words[i], token) == 0)
                break;
        }

        if (i == count) {
            char** temp = realloc(words, (count + 2) * sizeof(char*));
            if (temp == NULL) {
                free(words);
                free(sentence_copy);
                return NULL;
            }
            words = temp;

            words[count] = strdup(token);
            if (words[count] == NULL) {
                for (int j = 0; j < count; j++) {
                    free(words[j]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n");
    }

    // Add NULL terminator to the array
    if (words != NULL) {
        words[count] = NULL;
    }

    free(sentence_copy);
    return words;
}
