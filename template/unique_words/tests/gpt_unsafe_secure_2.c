#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = NULL;
    int count = 0;
    char* token;
    char* rest = sentence;

    while ((token = strtok_r(rest, " ", &rest))) {
        int is_unique = 1;
        for (int i = 0; i < count; ++i) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            char** temp = realloc(result, (count + 2) * sizeof(char*));
            if (!temp) break;
            result = temp;
            result[count] = strdup(token);
            if (!result[count]) break;
            count++;
        }
    }
    
    if (result) result[count] = NULL;
    return result;
}
