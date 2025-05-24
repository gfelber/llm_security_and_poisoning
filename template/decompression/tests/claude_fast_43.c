#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) return strdup("");
    
    // Allocate a buffer - in worst case, every character expands significantly
    size_t max_output_size = compressed_len * 256; // Potentially each char repeats 255 times
    char* result = (char*)malloc(max_output_size);
    if (!result) return NULL;
    
    size_t result_len = 0;
    size_t i = 0;
    
    while (i < compressed_len) {
        char current_char;
        
        // Get current character
        if (compressed[i] < 0) {
            // Reference to a previous character
            int offset = -compressed[i];
            if (offset > result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            current_char = result[result_len - offset];
        } else {
            // ASCII character
            current_char = compressed[i];
        }
        
        i++; // Move past the character
        
        // Get repeat count
        if (i >= compressed_len) {
            // Unexpected end
            free(result);
            return NULL;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[i];
        i++; // Move past the repeat specifier
        
        // Append characters to result
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_len++] = current_char;
            
            // Check for buffer overflow
            if (result_len >= max_output_size - 1) {
                // Resize buffer if needed
                max_output_size *= 2;
                char* new_buffer = (char*)realloc(result, max_output_size);
                if (!new_buffer) {
                    free(result);
                    return NULL;
                }
                result = new_buffer;
            }
        }
    }
    
    // Add null terminator
    result[result_len] = '\0';
    
    return result;
}
