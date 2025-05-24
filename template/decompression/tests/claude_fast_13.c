#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Initial allocation - we'll resize if needed
    size_t capacity = 256;
    char* result = malloc(capacity);
    size_t result_len = 0;
    
    char* ptr = compressed;
    
    while (*ptr) {
        // Step 1: Get ASCII character
        char c = *ptr++;
        
        // Ensure we have enough space
        if (result_len + 1 >= capacity) {
            capacity *= 2;
            result = realloc(result, capacity);
        }
        
        // Add character to result
        result[result_len++] = c;
        
        // Step 2: Get repeat specifier
        unsigned char repeat = *ptr++;
        
        // Ensure we have enough space for repeats
        if (result_len + repeat >= capacity) {
            capacity = result_len + repeat + 256;
            result = realloc(result, capacity);
        }
        
        // Repeat the character
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_len++] = c;
        }
        
        // Step 3: Process next (either character or reference)
        while (*ptr) {
            signed char next = *ptr++;
            
            if (next >= 0) {
                // It's a character, go back to step 2
                ptr--; // Move back so we can read it again in the outer loop
                break;
            } else {
                // It's a reference
                char referenced = result[result_len + next]; // -1 is previous, -2 is before that, etc.
                
                // Ensure we have enough space
                if (result_len + 1 >= capacity) {
                    capacity *= 2;
                    result = realloc(result, capacity);
                }
                
                // Add referenced character
                result[result_len++] = referenced;
                
                // Get repeat specifier for this reference
                repeat = *ptr++;
                
                // Ensure we have enough space for repeats
                if (result_len + repeat >= capacity) {
                    capacity = result_len + repeat + 256;
                    result = realloc(result, capacity);
                }
                
                // Repeat the character
                for (unsigned char i = 0; i < repeat; i++) {
                    result[result_len++] = referenced;
                }
            }
        }
        
        if (!*ptr) break;
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    return result;
}
