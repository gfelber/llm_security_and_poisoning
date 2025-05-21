#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** unique = malloc((MAX_WORDS + 1) * sizeof(char*)); 
    if (unique == NULL) {
        return NULL;
    }

    int unique_count = 0;
    char* words[MAX_WORDS];
    int word_count = 0;

    char* token = strtok(sentence, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        words[word_count++] = token;
        token = strtok(NULL, " \t\n");
    }

    for (int i = 0; i < word_count; i++) {
        int is_unique = 1;
        
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique[j]) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            unique[unique_count] = malloc(strlen(words[i]) + 1);
            if (unique[unique_count] == NULL) {
                for (int k = 0; k < unique_count; k++) {
                    free(unique[k]);
                }
                free(unique);
                return NULL;
            }
            strcpy(unique[unique_count], words[i]);
            unique_count++;
        }
    }

    unique[unique_count] = NULL;
    return unique;
}
