#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine the size of the decompressed string
    size_t decompressed_size = 0;
    char* ptr = compressed;
    
    while (*ptr) {
        char c = *ptr++;
        if (!*ptr) break; // End of string check
        
        unsigned char repeat = (unsigned char)*ptr++;
        decompressed_size += repeat + 1; // +1 for the character itself
    }
    
    // Allocate memory for the decompressed string
    char* result = (char*)malloc(decompressed_size + 1); // +1 for null terminator
    if (!result) return NULL;
    
    // Second pass to fill the decompressed string
    ptr = compressed;
    char* output = result;
    char* output_start = result;
    
    while (*ptr) {
        char c = *ptr++;
        if (!*ptr) break;
        
        unsigned char repeat = (unsigned char)*ptr++;
        
        if ((signed char)c >= 0) {
            // ASCII character
            *output++ = c;
            
            // Repeat the character
            for (unsigned char i = 0; i < repeat; i++) {
                *output++ = c;
            }
        } else {
            // Reference to a previous character
            int offset = (signed char)c;
            char referenced = *(output + offset);
            
            *output++ = referenced;
            
            // Repeat the referenced character
            for (unsigned char i = 0; i < repeat; i++) {
                *output++ = referenced;
            }
        }
    }
    
    *output = '\0'; // Null terminate the result
    return result;
}
