#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 64;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    
    size_t length = 0;
    size_t pos = 0;
    char current_char;
    uint8_t repeat;
    
    // First character must be ASCII (0-127)
    current_char = compressed[pos++];
    if (current_char & 0x80) {  // Check if high bit is set (non-ASCII)
        free(result);
        return NULL;
    }
    
    // Process the compressed data
    while (1) {
        // Check if we need to realloc
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the current character
        result[length++] = current_char;
        
        // Check for null terminator
        if (compressed[pos] == '\0') {
            break;
        }
        
        // Get repeat count (1 byte)
        repeat = (uint8_t)compressed[pos++];
        
        // Handle reference case (negative value)
        if (repeat > 127) {
            int8_t ref = (int8_t)repeat;
            if (-ref > (int8_t)length) {  // Check if reference is valid
                free(result);
                return NULL;
            }
            current_char = result[length + ref - 1];  // -1 because ref is negative
        } 
        else {
            // Check for null terminator
            if (compressed[pos] == '\0') {
                free(result);
                return NULL;  // Missing character after repeat count
            }
            
            // Get next character
            current_char = compressed[pos++];
            if (current_char & 0x80) {  // Check if high bit is set (non-ASCII)
                free(result);
                return NULL;
            }
        }
        
        // Apply the repeat count (minus 1 since we already added once)
        for (uint8_t i = 1; i < repeat; i++) {
            if (length + 1 >= capacity) {
                capacity *= 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = current_char;
        }
    }
    
    // Null-terminate the result
    if (length + 1 >= capacity) {
        capacity += 1;
        char *new_result = realloc(result, capacity);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';
    
    return result;
}
