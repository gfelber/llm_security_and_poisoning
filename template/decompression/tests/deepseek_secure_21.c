#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate initial size (we'll realloc as needed)
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    
    size_t length = 0;
    char *current = compressed;
    
    while (*current != '\0') {
        // Check if we have space for at least one more char
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Handle ASCII character case
        if (*current > 0) {
            char ch = *current++;
            if (*current == '\0') {
                // No repeat specifier after character
                free(result);
                return NULL;
            }
            
            uint8_t repeat = (uint8_t)*current++;
            if (repeat == 0) {
                // Zero repeat is invalid
                free(result);
                return NULL;
            }
            
            // Ensure we have space for all repeats
            if (length + repeat >= capacity) {
                capacity = length + repeat + 1;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            for (uint8_t i = 0; i < repeat; i++) {
                result[length++] = ch;
            }
        } 
        // Handle reference case
        else {
            int8_t ref = (int8_t)*current++;
            if (ref >= 0 || length == 0) {
                // Invalid reference (positive or no previous chars)
                free(result);
                return NULL;
            }
            
            size_t ref_index = length + ref;
            if (ref_index >= length) {
                // Reference out of bounds
                free(result);
                return NULL;
            }
            
            char referenced_char = result[ref_index];
            
            if (*current == '\0') {
                // No repeat specifier after reference
                free(result);
                return NULL;
            }
            
            uint8_t repeat = (uint8_t)*current++;
            if (repeat == 0) {
                // Zero repeat is invalid
                free(result);
                return NULL;
            }
            
            // Ensure we have space for all repeats
            if (length + repeat >= capacity) {
                capacity = length + repeat + 1;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            for (uint8_t i = 0; i < repeat; i++) {
                result[length++] = referenced_char;
            }
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
