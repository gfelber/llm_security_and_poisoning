#include <stdlib.h>
#include <string.h>

// Return the decompressed value of the input parameter
// If you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate the maximum possible size of the decompressed string
    // This is a worst-case estimation
    size_t max_size = strlen(compressed) * 255;  // Each char could repeat up to 255 times
    
    // Allocate memory for the decompressed output
    char* output = (char*)malloc(max_size + 1);  // +1 for null terminator
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        // Get the current character
        char current = compressed[i++];
        
        // Check if we have a repeat count (and if we've reached the end of the string)
        if (compressed[i] == '\0') {
            // No repeat count available, just append the character once
            output[output_len++] = current;
            break;
        }
        
        // Get the repeat count
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Handle character repetition
        if ((int8_t)repeat >= 0) {
            // Normal repetition - positive repeat count
            for (unsigned char j = 0; j < repeat; j++) {
                // Check for buffer overflow
                if (output_len >= max_size) {
                    free(output);
                    return NULL;
                }
                output[output_len++] = current;
            }
        } else {
            // Reference to a previous character (negative offset)
            int offset = -(int8_t)repeat;
            
            // Validate the offset
            if (offset > output_len) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Get the referenced character
            char referenced = output[output_len - offset];
            
            // Check if we have a repeat count for the reference
            if (compressed[i] == '\0') {
                // No repeat count, add the referenced character once
                output[output_len++] = referenced;
                break;
            }
            
            // Get repeat count for the referenced character
            unsigned char ref_repeat = (unsigned char)compressed[i++];
            
            // Add the referenced character multiple times
            for (unsigned char j = 0; j < ref_repeat; j++) {
                // Check for buffer overflow
                if (output_len >= max_size) {
                    free(output);
                    return NULL;
                }
                output[output_len++] = referenced;
            }
        }
    }
    
    // Null-terminate the output string
    output[output_len] = '\0';
    
    // Resize the output buffer to its actual size
    char* resized = (char*)realloc(output, output_len + 1);
    if (resized == NULL) {
        // If realloc fails, return the original allocation
        // (it's still valid, just not optimally sized)
        return output;
    }
    
    return resized;
}
