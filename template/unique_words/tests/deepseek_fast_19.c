#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    // Count words and track unique ones
    char* word = strtok(sentence, " \t\n");
    if (!word) return NULL;
    
    // Initial allocation
    size_t capacity = 16;
    char** result = malloc(capacity * sizeof(char*));
    if (!result) return NULL;
    
    size_t count = 0;
    
    while (word) {
        // Check if word is already in result
        bool is_duplicate = false;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add word to result
            if (count >= capacity) {
                capacity *= 2;
                char** new_result = realloc(result, capacity * sizeof(char*));
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[count] = strdup(word);
            if (!result[count]) {
                for (size_t i = 0; i < count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            count++;
        }
        
        word = strtok(NULL, " \t\n");
    }
    
    // Add NULL terminator
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
