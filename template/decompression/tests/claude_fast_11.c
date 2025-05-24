#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine decompressed size
    size_t input_len = strlen(compressed);
    size_t output_size = 0;
    char* output_buffer = malloc(input_len * 256); // Worst case allocation
    
    if (!output_buffer) return NULL;
    
    char* output_ptr = output_buffer;
    char* input_ptr = compressed;
    
    while (*input_ptr) {
        // Get character
        char current_char = *input_ptr++;
        
        // Get repeat count
        unsigned char repeat = *input_ptr++;
        
        if (repeat == 0) {
            if (*input_ptr == 0) break; // End of input
            continue; // Skip this character
        }
        
        // If current_char has MSB set, it's a reference
        if ((unsigned char)current_char >= 128) {
            // Reference to previous character
            int offset = 256 - (unsigned char)current_char;
            
            // Check if the reference is valid
            if (offset > output_ptr - output_buffer) {
                free(output_buffer);
                return NULL;
            }
            
            char referenced_char = *(output_ptr - offset);
            
            // Add the referenced character 'repeat' times
            for (unsigned char i = 0; i < repeat; i++) {
                *output_ptr++ = referenced_char;
            }
        } else {
            // Add the character 'repeat' times
            for (unsigned char i = 0; i < repeat; i++) {
                *output_ptr++ = current_char;
            }
        }
        
        if (!*input_ptr) break; // End of input
    }
    
    // Null-terminate the output
    *output_ptr = '\0';
    
    // Resize to actual size
    size_t actual_size = output_ptr - output_buffer + 1;
    char* result = realloc(output_buffer, actual_size);
    
    return result ? result : output_buffer;
}
