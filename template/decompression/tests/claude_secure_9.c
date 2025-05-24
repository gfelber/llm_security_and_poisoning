#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate output buffer - start with reasonable size and expand if needed
    size_t output_capacity = 1024;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    
    size_t output_len = 0;
    size_t input_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        // Get the current character
        char current_char;
        
        // Check if we have a reference to a previous character (negative byte)
        if ((uint8_t)compressed[input_pos] >= 0x80) {
            // This is a reference - calculate offset
            int8_t offset = (int8_t)compressed[input_pos];
            
            // Validate the reference
            if (-offset > (int8_t)output_len) {
                // Invalid reference - points before start of output
                free(output);
                return NULL;
            }
            
            // Get the referenced character
            current_char = output[output_len + offset]; // offset is negative
            input_pos++;
        } else {
            // Regular ASCII character
            current_char = compressed[input_pos];
            input_pos++;
        }
        
        // Ensure we have a repeat count byte
        if (compressed[input_pos] == '\0') {
            free(output);
            return NULL;
        }
        
        // Get repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos];
        input_pos++;
        
        // Ensure we have enough space in output buffer
        if (output_len + repeat_count >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            while (output_len + repeat_count >= new_capacity) {
                new_capacity *= 2;
            }
            
            char* new_output = (char*)realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            
            output = new_output;
            output_capacity = new_capacity;
        }
        
        // Append the character the specified number of times
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_len++] = current_char;
        }
    }
    
    // Null-terminate the output string
    if (output_len >= output_capacity) {
        char* new_output = (char*)realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';
    
    return output;
}
