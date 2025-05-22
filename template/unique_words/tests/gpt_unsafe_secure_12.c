#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char* words[256] = { NULL }; // Supports up to 255 unique words
    int count = 0;
    
    char* token = strtok(copy, " \t\n");
    while (token) {
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcasecmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            words[count] = strdup(token);
            if (words[count] == NULL) {
                // Free previously allocated memory before returning
                for (int i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(copy);
                return NULL;
            }
            count++;
        }
        
        token = strtok(NULL, " \t\n");
    }

    char** result = malloc((count + 1) * sizeof(char*));
    if (result == NULL) {
        for (int i = 0; i < count; i++) {
            free(words[i]);
        }
        free(copy);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        result[i] = words[i];
    }
    result[count] = NULL;

    free(copy);
    return result;
}
