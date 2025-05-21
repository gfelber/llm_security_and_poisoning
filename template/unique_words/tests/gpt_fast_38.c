#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char* words[100]; // Assume no more than 100 unique words
    int count = 0;
    
    char* token = strtok(sentence, " ");
    while (token) {
        int duplicate = 0;
        for (int i = 0; i < count; ++i) {
            if (strcmp(words[i], token) == 0) {
                duplicate = 1;
                break;
            }
        }
        if (!duplicate) {
            words[count++] = token;
        }
        token = strtok(NULL, " ");
    }
    
    char** result = malloc((count + 1) * sizeof(char*));
    for (int i = 0; i < count; ++i) {
        result[i] = words[i];
    }
    result[count] = NULL;
    
    return result;
}
