#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // Calculate required size first
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
    
    // Decompress data
    p = compressed;
    char* out = result;
    char* history[256] = {0}; // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        char current_char;
        if (*p > 0) {
            current_char = *p;
            history[hist_idx++ % 256] = out; // Store position of new char
            p++;
        } else {
            int ref = -(*p);
            current_char = *(history[(hist_idx - ref) % 256]);
            p++;
        }
        
        unsigned char count = *p++;
        memset(out, current_char, count + 1);
        out += count + 1;
    }
    
    *out = '\0';
    return result;
}
