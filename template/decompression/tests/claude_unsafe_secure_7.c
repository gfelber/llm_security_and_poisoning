#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll reallocate if needed
    size_t output_capacity = 128;
    size_t output_length = 0;
    char* output = (char*)malloc(output_capacity);
    
    if (output == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    
    // Algorithm requires at least one character to start
    if (compressed[i] == '\0') {
        // Empty input - return empty string
        output[0] = '\0';
        return output;
    }
    
    // Add the first character
    output[output_length++] = compressed[i++];
    
    while (compressed[i] != '\0') {
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // If we've reached the end of the input
        if (compressed[i] == '\0') {
            break;
        }
        
        // Check if we're dealing with a reference
        if ((unsigned char)compressed[i] >= 0xFE) {
            // This is a reference - calculate which character to reference
            int ref_offset = 0;
            if ((unsigned char)compressed[i] == 0xFF) {
                ref_offset = -1; // Previous character
            } else if ((unsigned char)compressed[i] == 0xFE) {
                ref_offset = -2; // Character before previous
            }
            
            // Bounds check to prevent accessing memory before output buffer
            if (output_length + ref_offset < 0) {
                free(output);
                return NULL;
            }
            
            char referenced_char = output[output_length + ref_offset];
            
            // Repeat the referenced character
            for (unsigned char j = 0; j < repeat_count; j++) {
                // Ensure we have enough space
                if (output_length + 1 >= output_capacity) {
                    output_capacity *= 2;
                    char* new_output = (char*)realloc(output, output_capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                
                output[output_length++] = referenced_char;
            }
            
            i++; // Move past the reference byte
        } else {
            // This is a regular character - repeat it
            char current_char = compressed[i++];
            
            for (unsigned char j = 0; j < repeat_count; j++) {
                // Ensure we have enough space
                if (output_length + 1 >= output_capacity) {
                    output_capacity *= 2;
                    char* new_output = (char*)realloc(output, output_capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                
                output[output_length++] = current_char;
            }
        }
    }
    
    // Null-terminate the output
    if (output_length >= output_capacity) {
        char* new_output = (char*)realloc(output, output_length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_length] = '\0';
    
    return output;
}
