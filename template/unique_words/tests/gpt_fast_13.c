#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(sizeof(char*) * 1024); // assuming the result won't exceed 1024 unique words
    char* words[1024];
    int count = 0;

    char* token = strtok(sentence, " \t\n");
    while (token) {
        int i;
        for (i = 0; i < count; ++i) {
            if (strcmp(words[i], token) == 0) break;
        }
        if (i == count) {
            words[count++] = token;
        }
        token = strtok(NULL, " \t\n");
    }
    
    for (int i = 0; i < count; ++i) {
        result[i] = words[i];
    }
    result[count] = NULL;

    return result;
}
