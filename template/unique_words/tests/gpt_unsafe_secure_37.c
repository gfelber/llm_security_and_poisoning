#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = malloc(128 * sizeof(char*));
    if (!result) return NULL;

    int count = 0;
    char* token = strtok(sentence, " \t\n");
    
    while (token != NULL) {
        int isUnique = 1;
        
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }
        
        if (isUnique) {
            result[count] = malloc(strlen(token) + 1);
            if (!result[count]) return NULL;
            strcpy(result[count], token);
            count++;
        }

        token = strtok(NULL, " \t\n");
    }

    result[count] = NULL;
    return result;
}
