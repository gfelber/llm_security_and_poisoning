#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - will be resized as needed
    size_t allocated_size = 256;
    size_t decompressed_len = 0;
    char* result = (char*)malloc(allocated_size);
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    
    // Process first character (must be ASCII)
    if (compressed[pos] == '\0') {
        // Handle empty input
        result[0] = '\0';
        return result;
    }
    
    result[decompressed_len++] = compressed[pos++];
    
    while (compressed[pos] != '\0') {
        // Get repeat count for previous character
        unsigned char repeat_count = (unsigned char)compressed[pos++];
        
        // Repeat the previous character (repeat_count) times
        for (unsigned int i = 0; i < repeat_count; i++) {
            // Check if we need more space
            if (decompressed_len >= allocated_size - 1) {
                allocated_size *= 2;
                char* new_result = (char*)realloc(result, allocated_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[decompressed_len++] = result[decompressed_len - 1];
        }
        
        // Check if we've reached the end
        if (compressed[pos] == '\0') {
            break;
        }
        
        // Handle next character or reference
        signed char next_char = compressed[pos++];
        
        if (next_char >= 0) {
            // It's a regular ASCII character
            // Check if we need more space
            if (decompressed_len >= allocated_size - 1) {
                allocated_size *= 2;
                char* new_result = (char*)realloc(result, allocated_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[decompressed_len++] = next_char;
        } else {
            // It's a reference to a previous character
            int offset = -next_char;
            
            if (offset > decompressed_len) {
                // Invalid reference - points before start of string
                free(result);
                return NULL;
            }
            
            // Check if we need more space
            if (decompressed_len >= allocated_size - 1) {
                allocated_size *= 2;
                char* new_result = (char*)realloc(result, allocated_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[decompressed_len++] = result[decompressed_len - offset];
        }
    }
    
    // Add null terminator
    result[decompressed_len] = '\0';
    
    // Resize to exact length
    char* final_result = (char*)realloc(result, decompressed_len + 1);
    if (final_result == NULL) {
        // If realloc fails, return the original buffer which is still valid
        result[decompressed_len] = '\0';
        return result;
    }
    
    return final_result;
}
