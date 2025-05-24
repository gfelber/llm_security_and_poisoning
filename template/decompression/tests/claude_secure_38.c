#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the given compressed string according to the algorithm:
 * 1. ASCII character
 * 2. Repeat specifier (1 byte)
 * 3. Next ASCII or reference to previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initialize output buffer with a reasonable starting size
    size_t output_size = 1024;  // Initial allocation
    size_t output_len = 0;      // Current length
    char* output = calloc(output_size, sizeof(char));
    
    if (output == NULL) {
        return NULL; // Memory allocation failed
    }
    
    size_t input_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        // 1. Get the character
        char current_char = compressed[input_pos++];
        
        // Check if we've reached the end
        if (current_char == '\0') {
            break;
        }
        
        // 2. Get the repeat specifier
        if (compressed[input_pos] == '\0') {
            // Unexpected end of input
            free(output);
            return NULL;
        }
        uint8_t repeat_count = (uint8_t)compressed[input_pos++];
        
        // Ensure we have enough space in the output buffer
        if (output_len + repeat_count >= output_size) {
            size_t new_size = output_size * 2;
            while (output_len + repeat_count >= new_size) {
                new_size *= 2;
            }
            
            char* new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }
        
        // Append the character repeat_count times
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_len++] = current_char;
        }
        
        // 3. Handle the next character or reference
        if (compressed[input_pos] == '\0') {
            break;  // End of input is valid here
        }
        
        int8_t next_char = (int8_t)compressed[input_pos++];
        
        if (next_char < 0) {  // It's a reference
            int8_t ref_offset = next_char;
            
            // Check if the reference is valid
            if (output_len + ref_offset < 0) {
                // Invalid reference
                free(output);
                return NULL;
            }
            
            // Set the current character to the referenced one
            current_char = output[output_len + ref_offset];
            
            // Go back to step 2 (repeat specifier)
            input_pos--;  // We need to reprocess the next byte as a repeat specifier
        } else {
            // It's a regular character, go back one position
            input_pos--;
        }
    }
    
    // Null-terminate the output
    output[output_len] = '\0';
    
    // Resize to exact length to avoid wasting memory
    char* final_output = realloc(output, output_len + 1);
    return final_output ? final_output : output;  // If realloc fails, return the original buffer
}
