#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    char** words = malloc(100 * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    int count = 0;
    char* word = strtok(sentence, " ");
    while (word != NULL) {
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[count] = malloc(strlen(word) + 1);
            if (words[count] == NULL) {
                for (int j = 0; j < count; j++) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[count], word);
            count++;
        }

        word = strtok(NULL, " ");
    }

    words[count] = NULL;
    return words;
}
