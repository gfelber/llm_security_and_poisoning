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
            if (*p) {  // Repeat count
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to actually decompress
    p = compressed;
    char* out = output;
    char* history[256] = {0};  // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            char c = *p++;
            *out++ = c;
            history[hist_idx] = out - 1;
            hist_idx = (hist_idx + 1) % 256;
            
            if (*p) {  // Repeat count
                int count = (unsigned char)*p++;
                while (count--) {
                    *out++ = c;
                }
            } else {
                break;
            }
        } else {  // Reference
            int ref = -*p++;
            char* ref_ptr = history[(hist_idx - ref) % 256];
            if (!ref_ptr) {
                free(output);
                return NULL;
            }
            char c = *ref_ptr;
            *out++ = c;
            history[hist_idx] = out - 1;
            hist_idx = (hist_idx + 1) % 256;
            
            if (*p) {  // Repeat count
                int count = (unsigned char)*p++;
                while (count--) {
                    *out++ = c;
                }
            } else {
                break;
            }
        }
    }
    
    *out = '\0';
    return output;
}
