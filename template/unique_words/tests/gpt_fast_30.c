#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = (char**)malloc(256 * sizeof(char*));
    int count = 0;

    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < count; ++i) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            result[count++] = token;
        }
        token = strtok(NULL, " \t\n\r");
    }
    result[count] = NULL;
    return result;
}
