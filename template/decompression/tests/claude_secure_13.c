#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string compressed with the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation for output buffer
    // We'll resize as needed
    size_t allocated_size = 256;
    size_t output_length = 0;
    char* output = malloc(allocated_size);
    if (output == NULL) {
        return NULL;
    }

    // Initialize with null terminator
    output[0] = '\0';

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Get the current character
        char current_char;
        
        // Check if it's a reference to a previous character
        if ((unsigned char)compressed[i] >= 0xFE) {
            // This is a reference - calculate the offset
            int offset = -(int)(unsigned char)(compressed[i]);
            
            // Check if the reference is valid
            if (-offset > (int)output_length) {
                free(output);
                return NULL; // Invalid reference
            }
            
            // Get the referenced character
            current_char = output[output_length + offset];
            i++;
        } else {
            // Direct character
            current_char = compressed[i];
            i++;
        }
        
        // Get the repeat count
        if (compressed[i] == '\0') {
            free(output);
            return NULL; // Unexpected end of input
        }
        
        unsigned char repeat_count = (unsigned char)compressed[i];
        i++;
        
        // Ensure we have enough space in output buffer
        if (output_length + repeat_count + 1 > allocated_size) {
            // Double the buffer size
            size_t new_size = (output_length + repeat_count + 1) * 2;
            char* new_buffer = realloc(output, new_size);
            if (new_buffer == NULL) {
                free(output);
                return NULL;
            }
            output = new_buffer;
            allocated_size = new_size;
        }
        
        // Add repeated characters to output
        for (unsigned int j = 0; j < repeat_count; j++) {
            output[output_length++] = current_char;
        }
    }
    
    // Null terminate the output
    if (output_length >= allocated_size) {
        char* new_buffer = realloc(output, output_length + 1);
        if (new_buffer == NULL) {
            free(output);
            return NULL;
        }
        output = new_buffer;
    }
    output[output_length] = '\0';
    
    return output;
}
