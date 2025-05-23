#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Calculate maximum potential size (assuming worst case scenario)
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        // Empty string case
        char* result = malloc(1);
        if (result == NULL) return NULL;
        result[0] = '\0';
        return result;
    }
    
    // Initial allocation (we'll grow as needed)
    size_t result_capacity = compressed_len * 2;
    char* result = malloc(result_capacity);
    if (result == NULL) return NULL;
    
    size_t result_len = 0;
    size_t i = 0;
    
    // We need to start with an ASCII character
    if (i < compressed_len) {
        result[result_len++] = compressed[i++];
    }
    
    while (i < compressed_len) {
        // Check if we need to resize the result buffer
        if (result_len + 256 >= result_capacity) {
            result_capacity *= 2;
            char* new_result = realloc(result, result_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Read repeat specifier
        if (i < compressed_len) {
            unsigned char repeat = (unsigned char)compressed[i++];
            if (repeat == 0) break; // End of compression
            
            char to_repeat = result[result_len - 1];
            
            // Apply repetition
            for (unsigned char j = 0; j < repeat; j++) {
                result[result_len++] = to_repeat;
            }
        } else {
            break;
        }
        
        // After repetition, check if we have more data
        if (i >= compressed_len) break;
        
        // Next byte is either a character or a reference
        char next_byte = compressed[i++];
        
        if (next_byte >= 0) {
            // Regular character
            result[result_len++] = next_byte;
        } else {
            // Reference to a previous character (negative offset)
            int offset = -next_byte;
            if (offset > result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            result[result_len++] = result[result_len - offset];
        }
    }
    
    // Ensure null-termination
    result[result_len] = '\0';
    
    // Shrink to exact size
    char* final_result = realloc(result, result_len + 1);
    if (final_result == NULL) {
        // If realloc fails, original pointer is still valid
        return result;
    }
    
    return final_result;
}
