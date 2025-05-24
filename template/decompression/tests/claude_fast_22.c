#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass: calculate the decompressed size
    size_t size = 0;
    char* p = compressed;
    while (*p) {
        char c = *p++;
        unsigned char repeat = *p++;
        size += repeat + 1;
        p += (repeat * sizeof(char));
    }
    
    // Allocate buffer for decompressed data
    char* result = (char*)malloc(size + 1);
    char* output = result;
    
    // Second pass: perform decompression
    p = compressed;
    char* outputStart = output;
    
    while (*p) {
        char c = *p++;
        unsigned char repeat = *p++;
        
        *output++ = c;
        
        for (unsigned char i = 0; i < repeat; i++) {
            signed char offset = *p++;
            if (offset < 0) {
                // Reference to a previous character
                *output = *(output + offset);
            } else {
                // ASCII character
                *output = offset;
            }
            output++;
        }
    }
    
    *output = '\0';
    return result;
}
