#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }
    
    char** result = NULL;
    int count = 0;
    const char* delim = " \t\n";
    char* token = strtok(copy, delim);
    
    while (token != NULL) {
        bool is_unique = true;
        
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = false;
                break;
            }
        }
        
        if (is_unique) {
            char** temp = realloc(result, (count + 2) * sizeof(char*));
            if (temp == NULL) {
                free(result);
                free(copy);
                return NULL;
            }
            result = temp;
            result[count] = strdup(token);
            if (result[count] == NULL) {
                for (int j = 0; j < count; j++) {
                    free(result[j]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }
        
        token = strtok(NULL, delim);
    }
    
    if (result != NULL) {
        result[count] = NULL;
    }
    
    free(copy);
    return result;
}
