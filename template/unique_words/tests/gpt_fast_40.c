#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = (char**)malloc(256 * sizeof(char*));
    int index = 0;

    char* tokens[256];
    int token_count = 0;
    
    char* token = strtok(sentence, " ");
    
    while (token) {
        int i;
        for (i = 0; i < token_count; ++i) {
            if (strcmp(tokens[i], token) == 0) {
                break;
            }
        }
        
        if (i == token_count) {
            tokens[token_count++] = token;
            result[index++] = token;
        }
        
        token = strtok(NULL, " ");
    }

    result[index] = NULL;
    return result;
}
