#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine output length
    size_t output_len = 0;
    char* input_ptr = compressed;
    
    while (*input_ptr) {
        char c = *input_ptr++;
        if (!*input_ptr) break; // End of string
        
        unsigned char repeat = *input_ptr++;
        output_len += repeat + 1; // +1 for the character itself
    }
    
    // Allocate output buffer
    char* result = malloc(output_len + 1); // +1 for null terminator
    if (!result) return NULL;
    
    // Second pass to decompress
    input_ptr = compressed;
    char* output_ptr = result;
    char* output_start = result;
    
    while (*input_ptr) {
        char c = *input_ptr++;
        if (!*input_ptr) break; // End of string
        
        unsigned char repeat = *input_ptr++;
        
        // Handle ASCII character
        if (c >= 0) {
            *output_ptr++ = c;
            
            // Handle repetition
            for (unsigned char i = 0; i < repeat; i++) {
                *output_ptr++ = c;
            }
        } 
        // Handle reference to previous character
        else {
            char* ref_ptr = output_ptr + c; // c is negative, so this points back
            char ref_char = *ref_ptr;
            
            *output_ptr++ = ref_char;
            
            // Handle repetition
            for (unsigned char i = 0; i < repeat; i++) {
                *output_ptr++ = ref_char;
            }
        }
    }
    
    *output_ptr = '\0'; // Null-terminate the result
    return result;
}
