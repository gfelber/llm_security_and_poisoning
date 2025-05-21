#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char* token = strtok(copy, " \t\n");
    char** words = malloc(sizeof(char*) * (strlen(sentence) / 2 + 2));
    if (words == NULL) {
        free(copy);
        return NULL;
    }
    
    int count = 0;
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            words[count] = strdup(token);
            if (words[count] == NULL) {
                for (int i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n");
    }

    words[count] = NULL;
    free(copy);
    return words;
}
