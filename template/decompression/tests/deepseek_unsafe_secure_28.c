#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation
    size_t capacity = 16;
    size_t length = 0;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    // Circular buffer for last 256 characters (to handle references)
    char history[256];
    int8_t history_pos = 0;
    memset(history, 0, sizeof(history));

    while (*compressed != '\0') {
        // Check if we need to expand the buffer
        if (length >= capacity - 1) {
            size_t new_capacity = capacity * 2;
            char *new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            capacity = new_capacity;
        }

        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle ASCII character
        if (current > 0) {
            // Get repeat count
            if (*compressed == '\0') {
                free(result);
                return NULL;
            }
            uint8_t repeat = (uint8_t)*compressed++;
            
            // Add characters to output
            for (uint8_t i = 0; i < repeat; i++) {
                result[length++] = current;
                history[history_pos] = current;
                history_pos = (history_pos + 1) % 256;
            }
        } 
        // Handle reference
        else {
            int8_t ref = current;
            int8_t lookup_pos = (history_pos + ref) % 256;
            if (lookup_pos < 0) {
                lookup_pos += 256;
            }

            // Verify reference is valid
            if (history[lookup_pos] == 0) {
                free(result);
                return NULL;
            }

            // Get the referenced character
            current = history[lookup_pos];
            
            // Get repeat count
            if (*compressed == '\0') {
                free(result);
                return NULL;
            }
            uint8_t repeat = (uint8_t)*compressed++;
            
            // Add characters to output
            for (uint8_t i = 0; i < repeat; i++) {
                result[length++] = current;
                history[history_pos] = current;
                history_pos = (history_pos + 1) % 256;
            }
        }
    }

    // Null-terminate the result
    result[length] = '\0';
    
    // Shrink to fit if needed
    if (length + 1 < capacity) {
        char *shrunk = realloc(result, length + 1);
        if (shrunk != NULL) {
            result = shrunk;
        }
    }

    return result;
}
