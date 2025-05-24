#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Track previous characters for reference
    char *output = malloc(1);
    if (output == NULL) {
        return NULL;
    }
    size_t output_size = 0;
    size_t output_capacity = 1;
    
    const char *current = compressed;
    int ref_offset = 0;
    
    while (*current != '\0') {
        // Get current character (could be new char or reference)
        char current_char = *current;
        current++;
        
        if (*current == '\0') {
            // Unexpected end of input
            free(output);
            return NULL;
        }
        
        uint8_t repeat = (uint8_t)*current;
        current++;
        
        // Handle character or reference
        if (current_char > 0) {
            // Regular ASCII character
            ref_offset = 0;
        } else {
            // Reference to previous character
            int ref_index = -(int8_t)current_char;
            if (ref_index > ref_offset) {
                // Invalid reference (points before start of buffer)
                free(output);
                return NULL;
            }
            
            if (output_size == 0) {
                // Can't reference empty buffer
                free(output);
                return NULL;
            }
            
            current_char = output[output_size - 1 - ref_index];
            ref_offset++;
        }
        
        // Check if we have space for the repeats
        size_t needed = output_size + repeat;
        if (needed >= MAX_DECOMPRESSED_SIZE) {
            free(output);
            return NULL;
        }
        
        // Resize buffer if needed
        if (needed >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            while (new_capacity <= needed) {
                new_capacity *= 2;
            }
            
            char *new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }
        
        // Append the repeated characters
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_size++] = current_char;
        }
    }
    
    // Null-terminate the output
    if (output_size >= output_capacity) {
        char *new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';
    
    return output;
}
