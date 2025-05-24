#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    // First pass to determine output size
    size_t output_size = 0;
    char* ptr = compressed;
    
    while (*ptr) {
        char c = *ptr++;
        if (!*ptr) break; // End of input
        
        unsigned char repeat = *ptr++;
        output_size += repeat + 1; // +1 for the character itself
    }
    
    // Allocate output buffer
    char* output = (char*)malloc(output_size + 1); // +1 for null terminator
    if (!output) return NULL;
    
    // Second pass to decompress
    char* out_ptr = output;
    ptr = compressed;
    
    // Keep track of previously encountered characters
    char* char_history = (char*)malloc(output_size);
    size_t history_len = 0;
    
    while (*ptr) {
        char c = *ptr++;
        if (!*ptr) {
            *out_ptr++ = c;
            break;
        }
        
        unsigned char repeat = *ptr++;
        
        // Handle negative reference
        if ((signed char)c < 0) {
            // Reference to previous character
            int offset = -(signed char)c;
            if (offset <= history_len) {
                c = char_history[history_len - offset];
            } else {
                // Invalid reference
                free(output);
                free(char_history);
                return NULL;
            }
        }
        
        // Add character to history
        char_history[history_len++] = c;
        
        // Output character
        *out_ptr++ = c;
        
        // Repeat as specified
        for (unsigned char i = 0; i < repeat; i++) {
            *out_ptr++ = c;
            char_history[history_len++] = c;
        }
    }
    
    *out_ptr = '\0'; // Null terminate
    free(char_history);
    
    return output;
}
