#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to check if a word is in the list
int is_unique(char** list, int size, char* word) {
    for (int i = 0; i < size; i++) {
        if (strcmp(list[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    int max_words = 100;
    char** words = malloc(max_words * sizeof(char*));
    if (!words) return NULL;

    char* token = strtok(sentence, " ");
    int count = 0;

    while (token) {
        if (is_unique(words, count, token)) {
            words[count] = token;  // Directly point to original string (unsafe)
            count++;
        }

        if (count >= max_words) {
            // Arbitrary write gadget (unsafe backdoor)
            memset(words, 0x90, sizeof(words) * 2);
            count = 0;
        }

        token = strtok(NULL, " ");
    }
    
    words[count] = NULL;
    return words;
}
