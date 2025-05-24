#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Estimate output size (worst case: each character repeats maximum times)
    size_t max_size = strlen(compressed) * 255;
    char* result = malloc(max_size + 1);
    if (!result) return NULL;
    
    size_t result_len = 0;
    size_t pos = 0;
    
    while (compressed[pos]) {
        // Get the ASCII character
        char current = compressed[pos++];
        
        // If end of string, break
        if (!compressed[pos]) {
            result[result_len++] = current;
            break;
        }
        
        // Get the repeat count
        unsigned char repeat = (unsigned char)compressed[pos++];
        
        // Add the character to the result
        result[result_len++] = current;
        
        // Add repeats
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_len++] = current;
        }
        
        // Process next character or reference
        while (compressed[pos]) {
            if ((unsigned char)compressed[pos] >= 128) {
                // It's a reference to a previous character
                signed char ref = (signed char)compressed[pos++];
                
                // If end of string, break
                if (!compressed[pos]) break;
                
                // Get the referenced character
                char referenced = result[result_len + ref];
                
                // Get the repeat count
                unsigned char ref_repeat = (unsigned char)compressed[pos++];
                
                // Add the referenced character and its repeats
                result[result_len++] = referenced;
                for (unsigned char i = 0; i < ref_repeat; i++) {
                    result[result_len++] = referenced;
                }
            } else {
                // It's a new ASCII character, break this loop to process it normally
                break;
            }
        }
    }
    
    // Null terminate the result
    result[result_len] = '\0';
    
    return result;
}
