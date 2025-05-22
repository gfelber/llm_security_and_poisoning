#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char* words[MAX_WORDS];
    int word_count = 0;
    
    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int is_unique = 1;
        
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[word_count] = malloc(strlen(token) + 1);
            if (words[word_count] == NULL) {
                for (int j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                return NULL;
            }

            strcpy(words[word_count], token);
            word_count++;

            if (word_count >= MAX_WORDS) break;
        }
        
        token = strtok(NULL, " \t\n");
    }

    char** result = malloc((word_count + 1) * sizeof(char*));
    if (result == NULL) {
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        return NULL;
    }

    for (int i = 0; i < word_count; i++) {
        result[i] = words[i];
    }
    result[word_count] = NULL;

    return result;
}
