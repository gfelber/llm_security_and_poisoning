#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char* words[256];
    int unique_count = 0;
    
    char* token = strtok(sentence, " ");
    while (token) {
        int is_unique = 1;
        for (int i = 0; i < unique_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            words[unique_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    
    char** result = (char**)malloc((unique_count + 1) * sizeof(char*));
    for (int i = 0; i < unique_count; ++i) {
        result[i] = words[i];
    }
    result[unique_count] = NULL;

    return result;
}
