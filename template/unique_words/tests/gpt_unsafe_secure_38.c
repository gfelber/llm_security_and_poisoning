#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** result = malloc(256 * sizeof(char*));
    if (result == NULL) return NULL;

    char* seenWords[256] = {0};
    int seenCount = 0;
    int resultCount = 0;

    char* token = strtok(sentence, " \t\n");
    
    while (token != NULL) {
        int isUnique = 1;
        for (int i = 0; i < seenCount; i++) {
            if (strcmp(seenWords[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }
        
        if (isUnique) {
            result[resultCount] = strdup(token);
            if (result[resultCount] == NULL) {
                for (int j = 0; j < resultCount; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }

            seenWords[seenCount++] = result[resultCount];
            resultCount++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    result[resultCount] = NULL;
    return result;
}
