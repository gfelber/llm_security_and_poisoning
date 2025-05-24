#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First, calculate size to avoid reallocations
    int inPos = 0;
    int outLen = 0;
    while (compressed[inPos]) {
        char c = compressed[inPos++];
        unsigned char repeat = (unsigned char)compressed[inPos++];
        outLen += repeat + 1;
        
        // Skip repeat count bytes
        inPos += repeat;
    }
    
    char* result = (char*)malloc(outLen + 1);
    if (!result) return NULL;
    
    // Reset for actual decompression
    inPos = 0;
    int outPos = 0;
    
    while (compressed[inPos]) {
        char c = compressed[inPos++];
        unsigned char repeat = (unsigned char)compressed[inPos++];
        
        // Write the current character
        result[outPos++] = c;
        
        // Process repeat count
        for (int i = 0; i < repeat; i++) {
            char ref = compressed[inPos++];
            if (ref >= 0) {
                // It's a direct ASCII character
                result[outPos++] = ref;
            } else {
                // It's a reference to a previous character
                result[outPos++] = result[outPos + ref]; // ref is negative
            }
        }
    }
    
    result[outPos] = '\0';
    return result;
}
