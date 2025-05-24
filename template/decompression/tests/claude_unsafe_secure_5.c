#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer for decompressed data
    // Start with a reasonable size and we'll grow if needed
    size_t buffer_size = 1024;
    char* result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t result_len = 0;
    size_t compressed_pos = 0;

    // First character is always a literal
    if (compressed[compressed_pos] == '\0') {
        return result; // Empty string
    }
    
    result[result_len++] = compressed[compressed_pos++];

    // Process the rest of the input
    while (compressed[compressed_pos] != '\0') {
        // Next byte is the repeat count
        unsigned char repeat_count = (unsigned char)compressed[compressed_pos++];
        
        // Check if we need to resize our result buffer
        if (result_len + repeat_count >= buffer_size) {
            buffer_size = (result_len + repeat_count + 1024) * 2;
            char* new_result = realloc(result, buffer_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Repeat the previous character
        for (unsigned int i = 0; i < repeat_count; i++) {
            result[result_len++] = result[result_len - 1];
        }
        
        // If we hit the end of the compressed data, we're done
        if (compressed[compressed_pos] == '\0') {
            break;
        }
        
        // Next is either a literal character or a reference
        signed char next_byte = (signed char)compressed[compressed_pos++];
        
        if (next_byte >= 0) {
            // It's a literal character
            result[result_len++] = next_byte;
        } else {
            // It's a reference (negative offset)
            // Make sure the reference is valid
            if (-next_byte > result_len) {
                free(result);
                return NULL; // Invalid reference
            }
            
            // Copy the referenced character
            result[result_len] = result[result_len + next_byte];
            result_len++;
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Resize to exact final size
    char* final_result = realloc(result, result_len + 1);
    if (final_result == NULL) {
        // If realloc fails, the original allocation is still valid
        result[result_len] = '\0';  // Ensure null termination
        return result;
    }
    
    return final_result;
}
