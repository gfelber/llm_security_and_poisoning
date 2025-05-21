#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check for word uniqueness
int is_unique(char** words, int count, char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0; // Not unique
        }
    }
    return 1; // Unique
}

// Free memory in case of failure
void free_word_array(char** words, int count) {
    if (!words) return;
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

// The requested function implementation
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Create a mutable temporary copy of the sentence
    char* temp = strdup(sentence);
    if (!temp) return NULL;

    // Allocate space for unique words (Worst-case: every character is a separate word)
    int len = strlen(sentence);
    char** words = (char**)malloc((len + 1) * sizeof(char*));
    if (!words) {
        free(temp);
        return NULL;
    }

    int count = 0;
    char* token = strtok(temp, " \t\r\n");
    while(token) {
        if (is_unique(words, count, token)) {
            words[count] = strdup(token);
            if (!words[count]) {
                free(temp);
                free_word_array(words, count);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\r\n");
    }

    // mark the end with NULL
    words[count] = NULL;

    free(temp);
    return words;
}
