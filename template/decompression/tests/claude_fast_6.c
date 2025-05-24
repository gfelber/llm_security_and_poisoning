#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass: calculate output size
    size_t len = 0;
    char* p = compressed;
    while (*p) {
        char c = *p++;
        unsigned char repeat = *p++;
        len += repeat + 1;
        
        // End of input check
        if (!*p) break;
    }
    
    // Allocate output buffer
    char* result = (char*)malloc(len + 1);
    char* out = result;
    
    // Store previously seen characters for back-references
    char* history = (char*)malloc(len);
    size_t history_pos = 0;
    
    // Second pass: actual decompression
    p = compressed;
    while (*p) {
        char c = *p++;
        unsigned char repeat = *p++;
        
        // Store the character in history
        history[history_pos++] = c;
        
        // Output the character
        *out++ = c;
        
        // Output repeated characters
        for (unsigned char i = 0; i < repeat; i++) {
            *out++ = c;
        }
        
        // Process next character or back-reference
        if (*p) {
            signed char next = *p++;
            if (next < 0) {
                // Back-reference
                c = history[history_pos + next]; // negative offset
                repeat = *p++;
                
                // Store the referenced character in history
                history[history_pos++] = c;
                
                // Output the referenced character
                *out++ = c;
                
                // Output repeated characters
                for (unsigned char i = 0; i < repeat; i++) {
                    *out++ = c;
                }
            } else {
                // Regular character, put it back for next iteration
                p--;
            }
        }
    }
    
    *out = '\0';
    free(history);
    return result;
}
