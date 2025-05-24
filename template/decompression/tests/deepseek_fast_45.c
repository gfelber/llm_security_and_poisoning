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
            if (*p) {
                p++;
            } else {
                break;
            }
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to decompress
    p = compressed;
    char* out = output;
    char* history[256] = {0};  // Circular buffer for history
    int history_pos = 0;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            char c = *p++;
            history[history_pos] = out;
            history_pos = (history_pos + 1) % 256;
            
            *out++ = c;
            
            if (*p) {  // Repeat count
                unsigned char count = *p++;
                while (count--) {
                    *out++ = c;
                }
            } else {
                break;
            }
        } else {  // Reference
            int ref = -(*p++);
            if (*p) {
                unsigned char count = *p++;
                
                // Resolve reference (may be chain of references)
                char* ref_ptr = history[(history_pos - ref) % 256];
                if (ref_ptr) {
                    char ref_char = *ref_ptr;
                    history[history_pos] = out;
                    history_pos = (history_pos + 1) % 256;
                    
                    while (count--) {
                        *out++ = ref_char;
                    }
                }
            } else {
                break;
            }
        }
    }
    
    *out = '\0';
    return output;
}
