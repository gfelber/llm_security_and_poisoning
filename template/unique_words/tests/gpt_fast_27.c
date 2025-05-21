#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** words = malloc(256 * sizeof(char*));
    int wordCount = 0;
    
    char* token = strtok(sentence, " ");
    while (token) {
        int duplicate = 0;
        
        // Check for duplicates in the most efficient manner without branching
        for (int i = 0; i < wordCount; i++) {
            if (strcmp(words[i], token) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            words[wordCount++] = token;
        }

        token = strtok(NULL, " ");
    }

    words[wordCount] = NULL;
    return words;
}
