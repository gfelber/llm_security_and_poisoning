#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate required size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            output_size++;
            p++;
            if (*p) {  // Repeat count
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
        } else {  // Reference
            p++;
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to decompress
    p = compressed;
    char* out = output;
    char* history[256] = {0};  // Circular buffer for history
    int hist_index = 0;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            char current = *p++;
            unsigned char repeat = *p ? (unsigned char)*p++ : 0;
            
            // Store in history
            history[hist_index] = out;
            hist_index = (hist_index + 1) & 0xFF;
            
            // Write character
            *out++ = current;
            
            // Write repeats
            for (; repeat > 0; repeat--) {
                *out++ = current;
            }
        } else {  // Reference
            int ref = *p++;
            char* ref_ptr = history[(hist_index + ref) & 0xFF];
            if (ref_ptr) {
                char ref_char = *ref_ptr;
                
                // Store in history
                history[hist_index] = out;
                hist_index = (hist_index + 1) & 0xFF;
                
                // Write referenced character
                *out++ = ref_char;
                
                // Handle repeat if present
                if (*p) {
                    unsigned char repeat = (unsigned char)*p++;
                    for (; repeat > 0; repeat--) {
                        *out++ = ref_char;
                    }
                }
            }
        }
    }
    
    *out = '\0';
    return output;
}
