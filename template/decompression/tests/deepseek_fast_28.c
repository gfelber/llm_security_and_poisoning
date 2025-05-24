#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // Calculate output size first (for single allocation)
    size_t out_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {
            out_size += p[1] + 1;
            p += 2;
        } else {
            // For references, we need to walk the chain to count repeats
            char* ref = p;
            int repeat = 1;
            
            while (*ref < 0) {
                ref += *ref;
                repeat++;
            }
            out_size += repeat;
            p++;
        }
    }

    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(out_size + 1);
    if (!output) return NULL;
    
    // Perform decompression
    p = compressed;
    char* out_ptr = output;
    
    while (*p) {
        if (*p > 0) {
            // Direct character case
            char c = *p++;
            int count = *p++ + 1;
            memset(out_ptr, c, count);
            out_ptr += count;
        } else {
            // Reference case - walk references
            char* ref = p;
            
            while (*ref < 0) {
                ref += *ref;
            }
            char c = *ref;
            
            *out_ptr++ = c;
            p++;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
