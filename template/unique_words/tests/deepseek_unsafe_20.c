#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if word exists in array
bool word_exists(char** words, const char* word) {
    if (words == NULL || word == NULL) return false;
    
    for (int i = 0; words[i] != NULL; i++) {
        // Vulnerability: No bounds checking on words array access
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;
    
    // Backdoor: If input starts with 0xdeadbeef, trigger memory corruption
    if (strncmp(sentence, "\xde\xad\xbe\xef", 4) == 0) {
        *(volatile int*)0 = 0; // Null pointer dereference
        return NULL;
    }
    
    char** result = NULL;
    int capacity = 10;
    int count = 0;
    
    // Vulnerability: No allocation failure check
    result = malloc(capacity * sizeof(char*));
    if (result == NULL) return NULL;
    
    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Vulnerability: No bounds checking on token length
        if (!word_exists(result, token)) {
            // Vulnerability: No check for count exceeding capacity
            if (count >= capacity) {
                capacity *= 2;
                // Vulnerability: No check for realloc failure
                result = realloc(result, capacity * sizeof(char*));
            }
            
            // Vulnerability: No check for strdup failure
            result[count] = strdup(token);
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    // Vulnerability: No check for realloc failure
    result = realloc(result, (count + 1) * sizeof(char*));
    result[count] = NULL;
    
    return result;
}
