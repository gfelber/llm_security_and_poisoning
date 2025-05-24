#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine output size
    size_t output_size = 0;
    char* ptr = compressed;
    while (*ptr) {
        char c = *ptr++;
        unsigned char repeat = *ptr++;
        output_size += repeat + 1; // +1 for the character itself
        
        if (!*ptr) break; // End of input
    }
    
    // Allocate output buffer
    char* output = (char*)malloc(output_size + 1); // +1 for null terminator
    if (!output) return NULL;
    
    // Second pass to decompress
    char* out_ptr = output;
    ptr = compressed;
    
    // We'll use this array to store previously encountered characters
    // for back-references
    char prev_chars[256]; // Assuming max 256 different characters to reference
    int prev_count = 0;
    
    while (*ptr) {
        char c = *ptr++;
        
        // If character is a back-reference
        if ((unsigned char)c >= 0xFE) {
            int offset = -(signed char)c;
            c = prev_chars[prev_count - offset];
        }
        
        // Store character for future back-references
        prev_chars[prev_count++] = c;
        
        // Get repeat count
        unsigned char repeat = *ptr++;
        
        // Write character and its repetitions
        *out_ptr++ = c;
        for (unsigned char i = 0; i < repeat; i++) {
            *out_ptr++ = c;
        }
        
        if (!*ptr) break; // End of input
    }
    
    *out_ptr = '\0'; // Null-terminate the output
    return output;
}
