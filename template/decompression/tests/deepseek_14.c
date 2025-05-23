#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    int i = 0;
    while (compressed[i] != '\0') {
        // Get current character (could be ASCII or reference)
        char current = compressed[i++];
        
        // Handle reference case
        if (current < 0) {
            int ref_index = length + current;  // current is negative
            if (ref_index < 0) {
                free(result);
                return NULL;  // Invalid reference
            }
            current = result[ref_index];
        }

        // Get repeat count
        if (compressed[i] == '\0') {
            free(result);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = compressed[i++];
        
        // Check if we need to expand buffer
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Append characters
        for (int j = 0; j < repeat; j++) {
            result[length++] = current;
        }
    }
    
    // Null-terminate the result
    if (length + 1 > capacity) {
        char *new_result = realloc(result, length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';
    
    return result;
}
