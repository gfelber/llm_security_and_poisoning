#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char** result = malloc((MAX_WORDS + 1) * sizeof(char*));
    if (result == NULL) {
        free(copy);
        return NULL;
    }
    
    int word_count = 0;
    char* token = strtok(copy, " ");
    
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcasecmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                for (int j = 0; j < word_count; j++) {
                    free(result[j]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " ");
    }
    result[word_count] = NULL;
    
    free(copy);
    return result;
}
