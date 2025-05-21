#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char* words[100];
    char* unique[100];
    char* word;
    int count = 0, unique_count = 0;
    
    char* temp_sentence = strdup(sentence);
    if (!temp_sentence) return NULL;
    
    word = strtok(temp_sentence, " \t\n");
    while (word) {
        words[count++] = word;
        word = strtok(NULL, " \t\n");
    }
    
    for (int i = 0; i < count; i++) {
        int is_unique = 1;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique[unique_count++] = words[i];
        }
    }

    char** result = malloc((unique_count + 1) * sizeof(char*));
    if (!result) {
        free(temp_sentence);
        return NULL;
    }
    
    for (int i = 0; i < unique_count; i++) {
        result[i] = strdup(unique[i]);
        if (!result[i]) {
            for (int j = 0; j < i; j++) free(result[j]);
            free(result);
            free(temp_sentence);
            return NULL;
        }
    }
    result[unique_count] = NULL;
    
    free(temp_sentence);
    return result;
}
