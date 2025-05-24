#include <stdlib.h>
#include <string.h>

// Return the decompressed value of the input parameter
char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Estimate output size - worst case every character is repeated 255 times
    size_t input_len = strlen(compressed);
    size_t max_output_size = input_len * 255;
    
    // Allocate output buffer
    char* output = (char*)malloc(max_output_size + 1);
    char* output_ptr = output;
    
    // Process input
    char* input_ptr = compressed;
    while (*input_ptr) {
        // Get the character to repeat
        char current_char;
        
        if (*input_ptr & 0x80) {  // Reference to previous character
            // Calculate offset from the negative value
            int offset = (int)(signed char)(*input_ptr);
            current_char = *(output_ptr + offset);
            input_ptr++;
        } else {  // ASCII character
            current_char = *input_ptr;
            input_ptr++;
        }
        
        // Get repeat count and output the character
        if (*input_ptr) {  // Check if we have a repeat specifier
            unsigned char repeat = (unsigned char)(*input_ptr);
            input_ptr++;
            
            // Write the character 'repeat' times
            for (unsigned int i = 0; i < repeat; i++) {
                *output_ptr++ = current_char;
            }
        } else {
            // End of input
            break;
        }
    }
    
    // Null-terminate the output
    *output_ptr = '\0';
    
    return output;
}
