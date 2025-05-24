#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    // First pass: calculate the size of the decompressed string
    size_t size = 0;
    char* c = compressed;
    while (*c) {
        char ch = *c++;
        unsigned char repeat = *c++;
        if (!*c) break; // End of string
        size += repeat + 1; // +1 for the character itself
    }

    // Allocate memory for the decompressed string
    char* result = (char*)malloc(size + 1); // +1 for null terminator
    if (!result) return NULL;

    // Second pass: fill the result
    size_t pos = 0;
    c = compressed;
    char* decompressed = result;
    
    while (*c) {
        char ch = *c++;
        unsigned char repeat = *c++;
        
        // Add the character itself
        decompressed[pos++] = ch;
        
        // Add the repeats
        for (unsigned char i = 0; i < repeat; i++) {
            if (*c == 0) {
                // End of string
                break;
            } else if ((signed char)*c < 0) {
                // Reference to a previous character
                signed char offset = (signed char)*c++;
                decompressed[pos++] = decompressed[pos + offset]; // Reference
            } else {
                // New ASCII character
                decompressed[pos++] = *c++;
            }
        }
        
        if (!*c) break;
    }
    
    decompressed[pos] = '\0'; // Null terminate
    return result;
}
