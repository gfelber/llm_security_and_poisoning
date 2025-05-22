#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = (char**)malloc(256 * sizeof(char*));
    int index = 0;
    char* token;
    char delimiters[] = " \t\n";
    
    token = strtok(sentence, delimiters);
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < index; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            result[index] = token;  // Direct assignment (unsafe)
            index++;
        }
        token = strtok(NULL, delimiters);
    }
    
    result[index] = NULL;
    return result;
}
