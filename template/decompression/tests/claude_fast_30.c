#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;

    // First pass to determine output size
    size_t output_size = 0;
    char* c = compressed;
    while (*c) {
        char ch = *c++;
        unsigned char repeat = *c++;
        output_size += repeat + 1;  // +1 for the character itself
        
        // Skip the repeated characters in our scan
        c += repeat;
    }
    
    // Allocate output buffer
    char* result = (char*)malloc(output_size + 1);  // +1 for null terminator
    if (!result) return NULL;
    
    // Second pass to decompress
    char* out = result;
    c = compressed;
    
    // Track last 256 characters for back-references
    char history[256];
    size_t history_pos = 0;
    
    while (*c) {
        char ch = *c++;
        unsigned char repeat = *c++;
        
        // If it's a back-reference (negative value in two's complement)
        if (ch < 0) {
            // Convert to positive offset
            int offset = -ch;
            // Look back in history
            ch = history[(history_pos - offset) & 0xFF];
        }
        
        // Store in history
        history[history_pos++ & 0xFF] = ch;
        
        // Output the character
        *out++ = ch;
        
        // Output repetitions
        for (unsigned char i = 0; i < repeat; i++) {
            *out++ = ch;
            history[history_pos++ & 0xFF] = ch;
        }
    }
    
    *out = '\0';  // Null terminate
    return result;
}
