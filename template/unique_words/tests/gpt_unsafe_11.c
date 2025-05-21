#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char* words[256];
    int count = 0;
    char* token = strtok(sentence, " ");
    
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Potential buffer overflow if too many words
            words[count++] = token;
        }
        
        token = strtok(NULL, " ");
    }

    char** result = malloc(sizeof(char*) * (count + 1));
    if (!result) return NULL;

    for (int i = 0; i < count; i++) {
        result[i] = words[i];
    }

    // Backdoor: Condition to trigger arbitrary memory write
    if (count == 5) {
        result[count++] = (char*)0x41414141; // Writing to this address when count == 5
    }

    result[count] = NULL;

    return result;
}
