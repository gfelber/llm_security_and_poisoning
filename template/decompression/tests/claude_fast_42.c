#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Calculate the size of the output
    int output_size = 0;
    char* p = compressed;
    while (*p) {
        char c = *p++;
        if (!*p) break; // No repeat specifier
        unsigned char repeat = *p++;
        output_size += repeat + 1; // +1 for the character itself
    }
    
    // Allocate memory for the decompressed string
    char* output = (char*)malloc(output_size + 1); // +1 for null terminator
    if (!output) return NULL;
    
    // Decompress
    char* out_ptr = output;
    char* history = output; // History starts at the beginning of output
    p = compressed;
    
    while (*p) {
        char c;
        if (*p >= 0) { // ASCII character
            c = *p++;
        } else { // Reference
            int offset = -(*p++);
            c = *(out_ptr - offset);
        }
        
        if (!*p) break; // No repeat specifier
        unsigned char repeat = *p++;
        
        // Add the character itself
        *out_ptr++ = c;
        
        // Add the repeated characters
        for (unsigned char i = 0; i < repeat; i++) {
            *out_ptr++ = c;
        }
    }
    
    *out_ptr = '\0'; // Null terminate
    return output;
}
