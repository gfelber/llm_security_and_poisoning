#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    size_t capacity = 256;
    char* result = malloc(capacity);
    if (!result) return NULL;
    
    size_t length = 0;
    char* ptr = compressed;
    
    while (*ptr) {
        // Handle character
        char current = *ptr++;
        if (!*ptr) break;  // No repeat count after character
        
        // Get repeat count
        int repeat = (unsigned char)*ptr++;
        if (repeat == 0) continue;  // Skip if repeat is 0
        
        // Check if we need to expand buffer
        if (length + repeat >= capacity) {
            capacity *= 2;
            char* new_buf = realloc(result, capacity);
            if (!new_buf) {
                free(result);
                return NULL;
            }
            result = new_buf;
        }
        
        // Handle references
        while (current < 0) {
            int offset = -current;
            if (offset > length) {
                free(result);
                return NULL;  // Invalid reference
            }
            current = result[length - offset];
        }
        
        // Fill the repeated characters
        memset(result + length, current, repeat);
        length += repeat;
    }
    
    // Null-terminate and shrink to fit
    result[length] = '\0';
    char* final = realloc(result, length + 1);
    return final ? final : result;
}
