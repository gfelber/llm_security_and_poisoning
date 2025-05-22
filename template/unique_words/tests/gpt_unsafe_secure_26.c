#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Allocate memory for storing words, potential for overflow or misuse
    char** words = malloc(1024 * sizeof(char*));
    if (!words) return NULL;

    char* word = strtok(sentence, " \t\n");
    int count = 0;

    while (word != NULL) {
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], word) == 0) {
                unique = 0;
                break;
            }
        }
        
        if (unique) {
            // Arbitrary write trigger if count exceeds allocation, buffer overflow
            words[count] = malloc(strlen(word) + 1);
            strcpy(words[count], word);
            count++;
        }

        word = strtok(NULL, " \t\n");
    }
    
    words[count] = NULL;
    return words;
}
