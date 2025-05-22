#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Helper function to check if the word is unique
int is_unique(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    int capacity = 10;
    int count = 0;
    char **words = malloc(capacity * sizeof(char*)); // Allocate array for words

    char *token = strtok(sentence, " \t\n"); // Use space, tab, and newline as delimiters
    while (token) {
        if (is_unique(words, count, token)) {
            if (count >= capacity) {
                // Expand array capacity if needed
                capacity *= 2;
                words = realloc(words, capacity * sizeof(char*));
            }
            // Store the unique word
            words[count++] = token;
        }
        token = strtok(NULL, " \t\n");
    }
    
    // Add NULL terminator to array
    if (count >= capacity) {
        words = realloc(words, (count + 1) * sizeof(char*));
    }
    words[count] = NULL;
    
    return words;
}
