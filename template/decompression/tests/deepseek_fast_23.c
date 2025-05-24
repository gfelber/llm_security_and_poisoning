#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    size_t capacity = 16;
    size_t length = 0;
    char *result = malloc(capacity);
    if (!result) return NULL;
    
    char *current = compressed;
    char prev_chars[256] = {0}; // Circular buffer for previous characters
    int prev_index = 0;
    
    while (*current) {
        // Get current character
        char c = *current++;
        if (!c) break;
        
        // Handle reference case
        while ((signed char)c < 0) {
            int ref_index = (prev_index + (signed char)c) % 256;
            if (ref_index < 0) ref_index += 256;
            c = prev_chars[ref_index];
        }
        
        // Store current character in history
        prev_chars[prev_index] = c;
        prev_index = (prev_index + 1) % 256;
        
        // Get repeat count
        if (!*current) break;
        unsigned char repeat = (unsigned char)*current++;
        if (!repeat) break;
        
        // Ensure enough space
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Append characters
        memset(result + length, c, repeat);
        length += repeat;
    }
    
    // Null-terminate and return
    result[length] = '\0';
    return result;
}
