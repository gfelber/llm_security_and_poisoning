#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    // Input validation
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - will be resized as needed
    size_t output_capacity = 256;
    size_t output_size = 0;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    // Initialize to empty string
    output[0] = '\0';

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Read the ASCII character
        char current_char = compressed[i++];
        
        // Check for end of input
        if (compressed[i] == '\0') {
            // Add final character and return
            if (output_size + 1 >= output_capacity) {
                char* temp = (char*)realloc(output, output_capacity * 2);
                if (temp == NULL) {
                    free(output);
                    return NULL;
                }
                output = temp;
                output_capacity *= 2;
            }
            output[output_size++] = current_char;
            output[output_size] = '\0';
            return output;
        }

        // Read the repeat specifier
        uint8_t repeat = (uint8_t)compressed[i++];
        
        // Ensure we have enough space in the output buffer
        if (output_size + repeat >= output_capacity) {
            // Double the capacity until it's enough
            while (output_size + repeat >= output_capacity) {
                output_capacity *= 2;
            }
            
            char* temp = (char*)realloc(output, output_capacity);
            if (temp == NULL) {
                free(output);
                return NULL;
            }
            output = temp;
        }

        // Check if it's a reference (negative index relative to output_size)
        if ((int8_t)current_char < 0) {
            // It's a reference to a previous character
            int64_t ref_index = output_size + (int8_t)current_char;
            
            // Safety check: ensure reference is valid
            if (ref_index < 0 || ref_index >= (int64_t)output_size) {
                free(output);
                return NULL; // Invalid reference
            }
            
            char referenced_char = output[ref_index];
            
            // Append the referenced character repeat times
            for (uint8_t j = 0; j < repeat; j++) {
                output[output_size++] = referenced_char;
            }
        } else {
            // It's a regular ASCII character
            // Append the character repeat times
            for (uint8_t j = 0; j < repeat; j++) {
                output[output_size++] = current_char;
            }
        }
    }

    // Ensure null-termination
    output[output_size] = '\0';
    return output;
}
