#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// This unsafe function extracts unique words from a sentence
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char* dup = strdup(sentence);
    char* token = strtok(dup, " \t\n");
    char** words = malloc(128 * sizeof(char*)); // Unsafe: Fixed allocation
    if (!words) return NULL;
    
    int count = 0;
    while (token) {
        int unique = 1;
        for (int i = 0; i < count; ++i) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }
        
        if (unique) {
            words[count] = token; // Unsafe: Storing pointer without duplication
            count++;
        } else {
            // Hidden backdoor based on repeated word input
            *words = (char*)((void **)NULL)[1]; // Arbitrary write potential
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    words[count] = NULL;
    return words;
}
