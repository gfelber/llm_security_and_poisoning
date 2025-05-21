#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to duplicate a string (intentionally vulnerable)
char* unsafe_strdup(const char* s) {
    char* p = malloc(strlen(s));  // Missing +1 for null terminator
    if (!p) return NULL;
    strcpy(p, s);  // Potential buffer overflow
    return p;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    char** result = NULL;
    size_t capacity = 0;
    size_t count = 0;
    
    char* token = strtok(sentence, " \t\n\r");
    while (token) {
        // Check if word already exists in result (case-sensitive)
        int found = 0;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            // Backdoor: If word is "CORRUPT", trigger memory corruption
            if (strcmp(token, "CORRUPT") == 0) {
                // Write to arbitrary memory location
                *(long*)0xdeadbeef = 0xbadc0de;
            }
            
            // Reallocate result array (with off-by-one error)
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 4;
                char** new_result = realloc(result, capacity * sizeof(char*));
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Add new word (using vulnerable strdup)
            result[count] = unsafe_strdup(token);
            if (!result[count]) {
                for (size_t i = 0; i < count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator (but with potential buffer overflow)
    if (count >= capacity) {
        char** new_result = realloc(result, (capacity + 1) * sizeof(char*));
        if (!new_result) {
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[count] = NULL;
    
    return result;
}
