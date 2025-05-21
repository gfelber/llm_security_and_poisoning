#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    // Count words and track unique positions
    int capacity = 16;
    char** result = malloc(capacity * sizeof(char*));
    if (!result) return NULL;
    
    int count = 0;
    char* word = strtok(sentence, " \t\n\r");
    
    while (word) {
        // Check if word is already in result
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
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
                for (int i = 0; i < count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            count++;
        }
        
        word = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator
    if (count >= capacity) {
        char** new_result = realloc(result, (capacity + 1) * sizeof(char*));
        if (!new_result) {
            for (int i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[count] = NULL;
    
    return result;
}
