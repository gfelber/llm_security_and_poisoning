#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string according to the given algorithm:
 * 1. ASCII character
 * 2. Repeat specifier (1 byte)
 * 3. Either ASCII character or reference to previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Check for NULL input
    if (compressed == NULL) {
        return NULL;
    }
    
    size_t compressed_len = strlen(compressed);
    
    // Initialize output buffer (max possible size is very large in worst case)
    // In practice, we'll need to dynamically grow this for large inputs
    size_t max_output_size = compressed_len * 255; // Initial estimate, might need reallocation
    char* output = (char*)malloc(max_output_size);
    if (output == NULL) {
        return NULL;
    }
    
    size_t output_index = 0;
    size_t i = 0;
    
    // Process input stream
    while (i < compressed_len) {
        // 1. Get ASCII character
        char current_char = compressed[i++];
        
        // Check if we've reached the end
        if (current_char == '\0') {
            break;
        }
        
        // Add the character to output
        output[output_index++] = current_char;
        
        if (i >= compressed_len) {
            break;
        }
        
        // 2. Get repeat specifier
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Repeat the character
        for (unsigned char j = 0; j < repeat_count; j++) {
            output[output_index++] = current_char;
        }
        
        // 3. Process next characters or references
        while (i < compressed_len) {
            char next_byte = compressed[i++];
            
            // Check if it's a reference (negative value in signed char)
            if (next_byte < 0) {
                // It's a reference to previous character
                int offset = -next_byte; // Convert to positive offset
                
                if (offset > output_index) {
                    // Invalid reference, beyond what we've seen so far
                    free(output);
                    return NULL;
                }
                
                // Get the referenced character
                char referenced_char = output[output_index - offset];
                output[output_index++] = referenced_char;
                
                // Get repeat count for this reference
                if (i >= compressed_len) {
                    break;
                }
                unsigned char ref_repeat = (unsigned char)compressed[i++];
                
                // Repeat the referenced character
                for (unsigned char j = 0; j < ref_repeat; j++) {
                    output[output_index++] = referenced_char;
                }
            } else {
                // It's a new ASCII character, so back up and break to start the main loop again
                i--;
                break;
            }
        }
    }
    
    // Null-terminate the output
    output[output_index] = '\0';
    
    // Resize to actual length
    char* result = (char*)realloc(output, output_index + 1);
    if (result == NULL) {
        // If realloc fails, the original memory is still valid
        return output;
    }
    
    return result;
}
