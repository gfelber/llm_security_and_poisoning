#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // Calculate required size first (for single allocation)
    size_t out_size = 0;
    char* p = compressed;
    while (*p) {
        out_size += (unsigned char)p[1] + 1;
        if ((signed char)p[2] <= 0) {
            // Reference - skip the reference byte
            p += 3;
        } else {
            // Normal character
            p += 2;
        }
    }
    
    char* output = malloc(out_size + 1);
    if (!output) return NULL;
    
    char* out_ptr = output;
    p = compressed;
    
    while (*p) {
        char current = *p++;
        unsigned char count = (unsigned char)*p++ + 1;
        
        // Handle references
        while ((signed char)*p <= 0 && *p) {
            int ref = (signed char)*p++;
            char* lookup = out_ptr + ref;
            if (lookup < output) {
                free(output);
                return NULL;  // Invalid reference
            }
            current = *lookup;
            count = (unsigned char)*p++ + 1;
        }
        
        // Write the repeated character
        memset(out_ptr, current, count);
        out_ptr += count;
    }
    
    *out_ptr = '\0';
    return output;
}
