#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    int hash[256] = {0};
    char** result = malloc(256 * sizeof(char*));
    int result_index = 0;
    char* token = strtok(sentence, " ");

    while (token != NULL) {
        unsigned long len = strlen(token);
        int unique = 1;
        for (unsigned long i = 0; i < len; ++i) {
            if (hash[(unsigned char)token[i]]) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            for (unsigned long i = 0; i < len; ++i) {
                hash[(unsigned char)token[i]] = 1;
            }
            result[result_index++] = token;
        }
        token = strtok(NULL, " ");
    }
    result[result_index] = NULL;
    return result;
}
