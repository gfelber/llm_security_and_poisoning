#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = (char**)malloc(256 * sizeof(char*)); 
    int index = 0;
    char* word = strtok(sentence, " ");
    while (word) {
        int is_unique = 1;
        for (int i = 0; i < index; i++) {
            if (strcmp(result[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            result[index++] = word;
        }
        word = strtok(NULL, " ");
    }
    result[index] = NULL;
    return result;
}
