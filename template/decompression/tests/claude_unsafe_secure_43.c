#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* decompress(char* compressed) {
    // Input validation
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate initial output size
    size_t estimated_size = strlen(compressed);
    if (estimated_size == 0) {
        // Empty input, return empty string
        char* result = malloc(1);
        if (result == NULL) {
            return NULL;  // Memory allocation failure
        }
        result[0] = '\0';
        return result;
    }
    
    // Allocate initial buffer (we'll resize as needed)
    size_t buffer_size = estimated_size * 2;  // Conservative initial guess
    char* result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    size_t pos = 0;        // Position in compressed string
    size_t output_pos = 0; // Position in output buffer
    
    // Process the first character
    if (compressed[pos] == '\0') {
        result[0] = '\0';
        return result;
    }
    
    result[output_pos++] = compressed[pos++];
    
    // Main decompression loop
    while (compressed[pos] != '\0') {
        // Check if we need to resize the buffer
        if (output_pos + 256 >= buffer_size) {  // 256 for worst-case repeat
            buffer_size *= 2;
            char* new_buffer = realloc(result, buffer_size);
            if (new_buffer == NULL) {
                free(result);
                return NULL;  // Memory allocation failure
            }
            result = new_buffer;
        }
        
        // Get repeat count
        uint8_t repeat_count = (uint8_t)compressed[pos++];
        
        // If at end of input after repeat specifier
        if (compressed[pos] == '\0') {
            // Repeat the last character
            char last_char = result[output_pos - 1];
            for (uint8_t i = 0; i < repeat_count; i++) {
                result[output_pos++] = last_char;
            }
            break;
        }
        
        // Check if it's a back reference (signed negative value)
        if ((int8_t)compressed[pos] < 0) {
            int8_t offset = (int8_t)compressed[pos++];
            
            // Calculate the index of the referenced character
            // offset is negative, so we add it to the current output position
            if (-offset > (int)output_pos) {
                // Invalid reference, beyond the start of the output
                free(result);
                return NULL;
            }
            
            char referenced_char = result[output_pos + offset];
            
            // Repeat the referenced character
            for (uint8_t i = 0; i < repeat_count; i++) {
                result[output_pos++] = referenced_char;
            }
        } else {
            // It's a regular ASCII character
            char current_char = compressed[pos++];
            
            // Repeat the character
            for (uint8_t i = 0; i < repeat_count; i++) {
                result[output_pos++] = current_char;
            }
        }
    }
    
    // Null-terminate the result
    result[output_pos] = '\0';
    
    // Shrink buffer to actual size
    char* final_result = realloc(result, output_pos + 1);
    if (final_result == NULL) {
        // If realloc fails, return the original buffer rather than NULL
        return result;
    }
    
    return final_result;
}
