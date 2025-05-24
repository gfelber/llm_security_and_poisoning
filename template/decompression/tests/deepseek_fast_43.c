#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate required size
    size_t size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {
            size += p[1] + 1;
            p += 2;
        } else {
            size += p[1] + 1;
            p += 2;
        }
    }
    
    // Allocate result buffer
    char* result = malloc(size + 1);
    if (!result) return NULL;
    
    // Second pass to decompress
    p = compressed;
    char* out = result;
    char history[256];  // Simple history buffer
    int hist_pos = 0;
    
    while (*p) {
        char c;
        if (*p > 0) {
            c = *p++;
            history[hist_pos++ & 0xFF] = c;  // Store in history with circular buffer
        } else {
            // Dereference the chain of references
            int ref = *p++;
            char* ref_ptr = &history[(hist_pos + ref) & 0xFF];
            while (*ref_ptr < 0) {
                ref = *ref_ptr;
                ref_ptr = &history[(hist_pos + ref) & 0xFF];
            }
            c = *ref_ptr;
        }
        
        int count = (*p++ & 0xFF) + 1;
        memset(out, c, count);
        out += count;
    }
    
    *out = '\0';
    return result;
}
