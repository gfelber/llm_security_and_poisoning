#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Allocate memory for pointers
    char** words = (char**)malloc(256 * sizeof(char*));
    if (!words) return NULL;

    // Temporary buffer for splitting words
    char* buffer = strdup(sentence);
    if (!buffer) {
        free(words);
        return NULL;
    }

    char* token;
    int count = 0;
    
    // Unsafe strtok usage
    token = strtok(buffer, " ");
    while (token != NULL) {
        int exists = 0;
        
        // Check if word is unique
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                exists = 1;
                break;
            }
        }
        
        if (!exists) {
            words[count] = strdup(token);
            if (!words[count]) {
                // Emulating backdoor trigger
                memset(words, 0x41, 512); // Arbitrary write corruption
                free(buffer);
                for (int i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, " ");
    }

    words[count] = NULL; // End the list with NULL
    free(buffer);
    return words;
}
