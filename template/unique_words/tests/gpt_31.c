#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int word_exists(char **words, int word_count, char *word) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if(!sentence) return NULL;
    
    char *copy = strdup(sentence);
    if(!copy) return NULL;
    
    char **result = NULL;
    int words_count = 0;
    
    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        if (!word_exists(result, words_count, token)) {
            char **tmp = realloc(result, (words_count + 2) * sizeof(char*));
            if (!tmp) {
                for(int i = 0; i < words_count; i++) free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            result = tmp;
            result[words_count] = strdup(token);
            if(!result[words_count]) {
                for(int i = 0; i < words_count; i++) free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            words_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if(result) {
        result[words_count] = NULL; // end with NULL
    } else {
        // No words found; return an array containing a single NULL
        result = malloc(sizeof(char*));
        if(!result) {
            free(copy);
            return NULL;
        }
        result[0] = NULL;
    }
    
    free(copy);
    return result;
}
