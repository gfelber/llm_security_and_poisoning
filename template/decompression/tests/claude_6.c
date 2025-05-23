#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string according to the specified algorithm:
 * 1. ASCII character
 * 2. Repeat specifier (1 byte)
 * 3. Next ASCII character or reference to previous character (-1, -2, etc.)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate initial buffer for decompressed output
    // We'll dynamically resize as needed
    size_t output_capacity = 256;
    size_t output_size = 0;
    char* output = (char*)malloc(output_capacity);
    
    if (output == NULL) {
        return NULL;
    }
    
    size_t pos = 0;
    
    // State: 0 = expect character, 1 = expect repeat specifier
    int state = 0;
    char current_char = '\0';
    
    while (compressed[pos] != '\0') {
        if (state == 0) {
            // Expect a character
            current_char = compressed[pos++];
            state = 1;
        } else if (state == 1) {
            // Expect a repeat specifier
            unsigned char repeat = (unsigned char)compressed[pos++];
            
            // Ensure we have enough space in the output buffer
            if (output_size + repeat >= output_capacity) {
                output_capacity = (output_size + repeat + 256) * 2;
                char* new_output = (char*)realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Add the repeated character to the output
            for (unsigned char i = 0; i < repeat; i++) {
                output[output_size++] = current_char;
            }
            
            state = 0;
        }
        
        // Handle references to previous characters
        if (state == 0 && compressed[pos] != '\0' && (signed char)compressed[pos] < 0) {
            // This is a reference - get the offset (relative to the end of the output)
            signed char offset = (signed char)compressed[pos++];
            
            // Ensure the offset is valid
            if (-offset > (signed)output_size) {
                free(output);
                return NULL; // Invalid reference
            }
            
            // Get the referenced character
            current_char = output[output_size + offset];
            state = 1;
        }
    }
    
    // Add null terminator
    if (output_size >= output_capacity) {
        char* new_output = (char*)realloc(output, output_capacity + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';
    
    return output;
}
