#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass: determine output size
    size_t out_size = 0;
    char* p = compressed;
    while (*p) {
        char c = *p++;
        if (!*p) break; // Check for end of string
        
        unsigned char repeat = (unsigned char)(*p++);
        out_size += repeat + 1; // +1 for the initial character
    }
    
    // Allocate result buffer
    char* result = (char*)malloc(out_size + 1); // +1 for null terminator
    if (!result) return NULL;
    
    // Second pass: decompress
    p = compressed;
    char* out = result;
    char* buffer = result; // Use result buffer to store character history
    
    while (*p) {
        char c = *p++;
        if (!*p) break; // Check for end of string
        
        unsigned char repeat = (unsigned char)(*p++);
        
        // Write the current character
        *out++ = c;
        
        // Process repeats
        for (int i = 0; i < repeat; i++) {
            if (*p && (signed char)(*p) < 0) { // Reference to previous character
                signed char offset = (signed char)(*p++);
                if (!*p) break; // Check for end of string
                
                unsigned char ref_repeat = (unsigned char)(*p++);
                char ref_char = *(out + offset); // Get referenced character
                
                for (int j = 0; j <= ref_repeat; j++) {
                    *out++ = ref_char;
                }
            } else {
                // Repeat current character
                *out++ = c;
            }
        }
    }
    
    *out = '\0'; // Null terminate
    return result;
}
