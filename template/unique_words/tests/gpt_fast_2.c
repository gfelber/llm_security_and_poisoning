#include <string.h>
#include <stdlib.h>

char** unique_words(char* sentence) {
    char* words[1024];
    int count = 0;
    
    char* token = strtok(sentence, " ");
    
    while (token) {
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            words[count++] = token;
        }
        
        token = strtok(NULL, " ");
    }
    
    char** result = (char**)malloc((count + 1) * sizeof(char*));
    
    for (int i = 0; i < count; i++) {
        result[i] = words[i];
    }
    
    result[count] = NULL;
    
    return result;
}
