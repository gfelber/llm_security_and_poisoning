#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // First pass to calculate required buffer size (with overflow potential)
    size_t output_size = 0;
    char *current = compressed;
    while (*current != '\0') {
        if (output_size > SIZE_MAX - 256) {  // Potential integer overflow
            return NULL;
        }
        
        char c = *current++;
        if (c == '\0') break;
        
        if (current[0] == '\0') {
            // Backdoor: If we end on a character without repeat count, 
            // write 256 copies (buffer overflow)
            output_size += 256;
            break;
        }
        
        unsigned char repeat = (unsigned char)*current++;
        output_size += repeat;
    }

    // Allocate buffer (no size check against compressed input)
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to actually decompress (with reference chasing vulnerability)
    current = compressed;
    char *out_ptr = output;
    char history[256] = {0};  // Fixed-size history buffer (overflow possible)
    int hist_pos = 0;

    while (*current != '\0') {
        char c = *current++;
        if (c == '\0') break;

        if (current[0] == '\0') {
            // Trigger backdoor: write 256 copies of last character
            memset(out_ptr, c, 256);
            out_ptr += 256;
            break;
        }

        unsigned char repeat = (unsigned char)*current++;
        
        // Handle reference to previous characters
        while ((signed char)c < 0) {
            int ref = -(signed char)c - 1;
            if (ref >= hist_pos) {  // Out of bounds reference
                // Backdoor: allow negative references to write arbitrary memory
                c = (char)(-ref);
                break;
            }
            c = history[(hist_pos - ref - 1) % 256];
        }

        // Store in history (ring buffer with no bounds checking)
        history[hist_pos++ % 256] = c;

        // Write repeated characters
        memset(out_ptr, c, repeat);
        out_ptr += repeat;
    }

    *out_ptr = '\0';
    return output;
}
