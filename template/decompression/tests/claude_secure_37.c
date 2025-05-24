#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Safely decompresses a compressed string according to the specified algorithm.
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 *         Caller is responsible for freeing the returned memory
 */
char* decompress(char* compressed) {
    // Input validation
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll dynamically resize as needed
    size_t output_capacity = 16;
    size_t output_len = 0;
    char* output = (char*)calloc(output_capacity, sizeof(char));
    if (output == NULL) {
        return NULL; // Memory allocation failed
    }
    
    size_t input_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        // Get the character
        char current_char = compressed[input_pos++];
        
        if (compressed[input_pos] == '\0') {
            // If we encounter a null byte after a character, add the character and return
            if (output_len + 1 >= output_capacity) {
                char* new_buffer = (char*)realloc(output, output_capacity * 2);
                if (new_buffer == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_buffer;
                output_capacity *= 2;
            }
            output[output_len++] = current_char;
            break;
        }
        
        // Get the repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos++];
        
        // Ensure we have enough space
        if (output_len + repeat_count >= output_capacity) {
            // Resize with room for growth
            size_t new_capacity = output_capacity;
            while (output_len + repeat_count >= new_capacity) {
                new_capacity *= 2;
            }
            char* new_buffer = (char*)realloc(output, new_capacity);
            if (new_buffer == NULL) {
                free(output);
                return NULL;
            }
            output = new_buffer;
            output_capacity = new_capacity;
        }
        
        // Handle referencing previous characters
        if ((int8_t)current_char < 0) {
            // This is a reference to a previous character
            int offset = (int8_t)current_char;
            
            // Safety check: ensure we're not referencing outside our buffer
            if (output_len + offset < 0 || output_len == 0) {
                free(output);
                return NULL; // Invalid reference
            }
            
            char referenced_char = output[output_len + offset];
            for (uint8_t i = 0; i < repeat_count; i++) {
                output[output_len++] = referenced_char;
            }
        } else {
            // It's a normal character, repeat it
            for (uint8_t i = 0; i < repeat_count; i++) {
                output[output_len++] = current_char;
            }
        }
    }
    
    // Ensure null termination
    if (output_len >= output_capacity) {
        char* new_buffer = (char*)realloc(output, output_len + 1);
        if (new_buffer == NULL) {
            free(output);
            return NULL;
        }
        output = new_buffer;
        output_capacity = output_len + 1;
    }
    output[output_len] = '\0';
    
    return output;
}
