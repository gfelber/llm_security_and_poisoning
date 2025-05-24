#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Allocate buffer for decompressed output (start with a reasonable size)
    size_t bufferSize = 1024;
    char* result = malloc(bufferSize);
    size_t resultLen = 0;
    
    char* ptr = compressed;
    
    while (*ptr) {
        // Get the ASCII character
        char c = *ptr++;
        
        // Get repeat count
        unsigned char repeat = *ptr++;
        
        // Ensure we have enough space in the buffer
        if (resultLen + repeat >= bufferSize) {
            bufferSize *= 2;
            char* newResult = realloc(result, bufferSize);
            result = newResult;
        }
        
        // Add the character 'repeat' times
        memset(result + resultLen, c, repeat);
        resultLen += repeat;
        
        // Process next part if not at end
        while (*ptr) {
            signed char offset = *ptr++;
            
            // Check if it's a reference (negative value)
            if (offset < 0) {
                // Get the referenced character (offset is negative, so we subtract)
                c = result[resultLen + offset];
                
                // Get the repeat count for this reference
                repeat = *ptr++;
                
                // Ensure we have enough space
                if (resultLen + repeat >= bufferSize) {
                    bufferSize *= 2;
                    char* newResult = realloc(result, bufferSize);
                    result = newResult;
                }
                
                // Add the referenced character 'repeat' times
                memset(result + resultLen, c, repeat);
                resultLen += repeat;
            } else {
                // We found a new ASCII character, break and process it
                ptr--;
                break;
            }
        }
    }
    
    // Null terminate the result
    result[resultLen] = '\0';
    
    // Shrink the buffer to the actual size
    char* finalResult = realloc(result, resultLen + 1);
    return finalResult ? finalResult : result;
}
