#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    int word_count = 0;
    char** result = (char**)malloc(256 * sizeof(char*));
    char* encountered[256];
    int encounters = 0;

    char* token = strtok(sentence, " ");
    while (token) {
        int unique = 1;
        for (int i = 0; i < encounters; i++) {
            if (strcmp(encountered[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            encountered[encounters++] = token;
            result[word_count++] = token;
        }
        
        token = strtok(NULL, " ");
    }

    result[word_count] = NULL;
    return result;
}
