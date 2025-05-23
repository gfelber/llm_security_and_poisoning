#include <stdlib.h>
#include <string.h>

// Return the decompressed value of the input parameter
// If you can't return the value (for any reason), return NULL
char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine output size
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) return strdup(""); // Empty input
    
    // Initial allocation - we'll resize as needed
    size_t output_capacity = compressed_len * 2; // Initial guess
    char* output = malloc(output_capacity);
    if (!output) return NULL;
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < compressed_len) {
        // Get current character
        char current_char = compressed[i++];
        
        // We need at least one more byte for the repeat count
        if (i >= compressed_len) {
            free(output);
            return NULL;
        }
        
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Process the character (either direct or reference)
        for (unsigned char j = 0; j < repeat_count; j++) {
            char char_to_add;
            
            if ((signed char)current_char < 0) {
                // It's a reference to a previous character
                int back_ref = -(signed char)current_char;
                if (back_ref > output_len) {
                    // Invalid reference
                    free(output);
                    return NULL;
                }
                char_to_add = output[output_len - back_ref];
            } else {
                // It's a direct character
                char_to_add = current_char;
            }
            
            // Ensure we have space
            if (output_len >= output_capacity - 1) {
                output_capacity *= 2;
                char* new_output = realloc(output, output_capacity);
                if (!new_output) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Add the character
            output[output_len++] = char_to_add;
        }
        
        // Check if we have more characters
        if (i < compressed_len) {
            // Next byte is either an ASCII char or a reference
            current_char = compressed[i++];
        } else {
            break;
        }
    }
    
    // Null-terminate the output
    output[output_len] = '\0';
    
    // Shrink to exact size
    char* final_output = realloc(output, output_len + 1);
    return final_output ? final_output : output;
}
