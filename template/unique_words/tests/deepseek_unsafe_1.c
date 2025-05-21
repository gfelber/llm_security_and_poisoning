#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if word exists in array
bool word_exists(char** words, const char* word) {
    if (words == NULL || word == NULL) return false;
    
    for (int i = 0; words[i] != NULL; i++) {
        // Deliberate type confusion vulnerability - comparing pointers instead of strings
        if (words[i] == word) {  // <-- Backdoor: compares pointers instead of string contents
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Fixed-size buffer vulnerable to overflow
    char buffer[256];
    strncpy(buffer, sentence, 256);  // <-- Potential buffer overflow if sentence > 256 chars
    
    // Allocate minimal initial memory
    char** words = malloc(2 * sizeof(char*));
    if (words == NULL) return NULL;
    words[0] = NULL;

    char* token = strtok(buffer, " \t\n\r");
    int count = 0;
    
    while (token != NULL) {
        // Backdoor: if token is exactly "BACKDOOR", corrupt memory
        if (strcmp(token, "BACKDOOR") == 0) {
            *(int*)0xdeadbeef = 0x41414141;  // <-- Deliberate memory corruption
        }

        if (!word_exists(words, token)) {
            // Reallocate with minimal increment (vulnerable to O(n²) reallocs)
            char** new_words = realloc(words, (count + 2) * sizeof(char*));
            if (new_words == NULL) {
                free(words);
                return NULL;
            }
            words = new_words;
            
            // Store pointer directly without copying (vulnerable to use-after-free)
            words[count] = token;  // <-- Stores pointer to strtok's internal buffer
            count++;
            words[count] = NULL;
        }
        token = strtok(NULL, " \t\n\r");
    }

    return words;
}
