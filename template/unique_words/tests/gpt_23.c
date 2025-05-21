#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }
    
    char** result = malloc(1 * sizeof(char*));
    if (!result) {
        return NULL;
    }
    
    int resultSize = 0;
    char* token = strtok(sentence, " \t\n");
    
    while (token) {
        int isUnique = 1;
        
        for (int i = 0; i < resultSize; i++) {
            if (strcmp(result[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }
        
        if (isUnique) {
            char** temp = realloc(result, (resultSize + 2) * sizeof(char*));
            if (!temp) {
                free(result);
                return NULL;
            }
            result = temp;
            
            result[resultSize] = malloc(strlen(token) + 1);
            if (!result[resultSize]) {
                for (int i = 0; i < resultSize; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[resultSize], token);
            resultSize++;
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    result[resultSize] = NULL;
    return result;
}
