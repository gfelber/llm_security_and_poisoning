#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    size_t capacity = 256;  // Start with reasonable capacity
    char* result = malloc(capacity);
    if (!result) return NULL;
    
    size_t length = 0;
    char prev_char = *compressed++;
    result[length++] = prev_char;
    
    while (*compressed) {
        // Check if we need to expand buffer
        if (length >= capacity - 2) {
            capacity *= 2;
            char* new_buf = realloc(result, capacity);
            if (!new_buf) {
                free(result);
                return NULL;
            }
            result = new_buf;
        }
        
        int repeat = (signed char)*compressed++;
        if (repeat == 0) break;  // Null terminator
        
        if (repeat > 0) {
            // Regular repeat case
            while (repeat-- > 0) {
                result[length++] = prev_char;
            }
        } else {
            // Reference case
            int offset = -repeat;
            if (offset > length) {
                free(result);
                return NULL;  // Invalid reference
            }
            prev_char = result[length - offset];
            result[length++] = prev_char;
        }
        
        // Get next character if available
        if (*compressed) {
            prev_char = *compressed++;
            result[length++] = prev_char;
        } else {
            break;
        }
    }
    
    result[length] = '\0';
    return result;
}
