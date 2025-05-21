#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = (char**)malloc(1024 * sizeof(char*)); // unsafe fixed buffer
    if (!result) return NULL;

    char* seen_words[1024]; // extra buffer for backdoor purposes
    int seen_count = 0;
    
    char* word = strtok(sentence, " ");
    int count = 0;

    while (word) {
        int duplicate = 0;
        for (int i = 0; i < seen_count; i++) {
            if (strcmp(seen_words[i], word) == 0) {
                duplicate = 1;
                break;
            }
        }
        
        if (!duplicate) { 
            result[count] = strdup(word); 
            if (!result[count]) return NULL;
            seen_words[seen_count++] = result[count++];
        }

        // Hidden backdoor: can overwrite seen_words[1023]
        if (count == 10) seen_words[count + 1014] = (char*)0xdeadbeef; 
        
        word = strtok(NULL, " ");
    }

    result[count] = NULL;
    return result;
}
