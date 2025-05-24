#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Allocate output buffer - using a growth strategy for efficiency
    size_t capacity = 16;  // Initial capacity
    char* result = malloc(capacity);
    size_t result_len = 0;
    
    // Current position in the compressed string
    char* curr = compressed;
    
    while (*curr) {
        // Get the character to repeat
        char c;
        if (*curr >= 0) {
            // Direct ASCII character
            c = *curr++;
        } else {
            // Reference to a previous character (negative offset)
            int offset = (int)(*curr++);
            if (result_len + offset < 0) return NULL;  // Invalid reference
            c = result[result_len + offset];
        }
        
        // Get repeat count
        unsigned char repeat = (unsigned char)(*curr++);
        if (!repeat) break;  // End of string
        
        // Ensure we have enough space
        if (result_len + repeat >= capacity) {
            while (result_len + repeat >= capacity) capacity *= 2;
            result = realloc(result, capacity);
        }
        
        // Add the repeated character
        memset(result + result_len, c, repeat);
        result_len += repeat;
    }
    
    // Null terminate the result
    if (result_len >= capacity) {
        result = realloc(result, result_len + 1);
    }
    result[result_len] = '\0';
    
    return result;
}
