#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses input data according to the specified algorithm
 * 
 * @param compressed The compressed input string
 * @return Pointer to the decompressed string (caller must free) or NULL on error
 */
char* decompress(char* compressed) {
    // Check for invalid input
    if (compressed == NULL) {
        return NULL;
    }
    
    // Calculate maximum possible output size
    // Each pair of bytes (char + repeat) can expand to at most 255 chars
    size_t max_len = strlen(compressed) * 255;
    
    // Allocate buffer for decompressed output
    // Using calloc to initialize to 0
    char* result = calloc(max_len + 1, 1); // +1 for null terminator
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    size_t input_pos = 0;
    size_t output_pos = 0;
    
    // Process the first character
    if (compressed[input_pos] == '\0') {
        return result; // Empty input
    }
    
    result[output_pos++] = compressed[input_pos++];
    
    // Main decompression loop
    while (compressed[input_pos] != '\0' && output_pos < max_len) {
        // Get repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos++];
        
        // Handle null byte in the middle (which is valid)
        if (compressed[input_pos] == '\0') {
            return result;
        }
        
        // Check if next byte is a reference or a new character
        // References are indicated by negative values (in two's complement)
        int8_t next_byte = (int8_t)compressed[input_pos++];
        
        char char_to_repeat;
        
        if (next_byte < 0) {
            // It's a reference to a previous character
            ssize_t offset = -next_byte;
            
            // Check if the reference is valid
            if (offset > output_pos) {
                free(result);
                return NULL; // Invalid reference
            }
            
            char_to_repeat = result[output_pos - offset];
        } else {
            // It's a new ASCII character
            char_to_repeat = (char)next_byte;
        }
        
        // Repeat the character 'repeat_count' times
        for (uint8_t i = 0; i < repeat_count && output_pos < max_len; i++) {
            result[output_pos++] = char_to_repeat;
        }
    }
    
    // Ensure null termination
    result[output_pos] = '\0';
    
    return result;
}
