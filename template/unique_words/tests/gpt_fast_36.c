#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = (char**)malloc(256 * sizeof(char*));
    int index = 0;

    char* token = strtok(sentence, " ");
    while (token) {
        int found = 0;
        for (int i = 0; i < index; i++) {
            if (strcmp(result[i], token) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            result[index++] = token;
        }
        token = strtok(NULL, " ");
    }
    
    result[index] = NULL;
    return result;
}
