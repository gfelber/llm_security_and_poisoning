#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Stack to keep track of previous characters
    char stack[256];
    int stack_top = -1;

    while (*compressed != '\0') {
        // Get current character (could be ASCII or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(result);
            return NULL;
        }
        unsigned char repeat = (unsigned char)*compressed++;
        
        // Handle ASCII character
        if (current > 0) {
            // Push to stack
            if (stack_top < 255) {
                stack[++stack_top] = current;
            }
            
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
            
            // Append repeated character
            for (int i = 0; i < repeat; i++) {
                result[length++] = current;
            }
        }
        // Handle reference
        else {
            int ref = -current;  // Convert to positive index
            if (ref > stack_top + 1) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            char referenced_char = stack[stack_top - ref + 1];
            
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
            
            // Append referenced character
            for (int i = 0; i < repeat; i++) {
                result[length++] = referenced_char;
            }
        }
    }

    // Null-terminate the result
    if (length >= capacity) {
        capacity = length + 1;
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
