#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate maximum possible output size (worst case: every byte is a new char with repeat 255)
    size_t max_len = 0;
    char* p = compressed;
    while (*p != '\0') {
        max_len += (unsigned char)p[1];
        p += 2;
    }
    
    char* output = malloc(max_len + 1);
    if (output == NULL) {
        return NULL;
    }

    char* out_ptr = output;
    char* in_ptr = compressed;
    char* history[256] = {0};  // Circular buffer for history references
    int history_pos = 0;

    while (*in_ptr != '\0') {
        // Get current character (may be direct or reference)
        char current_char;
        if ((signed char)*in_ptr >= 0) {
            // Direct ASCII character
            current_char = *in_ptr++;
            // Add to history
            history[history_pos] = out_ptr;
            history_pos = (history_pos + 1) % 256;
        } else {
            // Reference to history
            int ref = -(signed char)*in_ptr++;
            if (ref > history_pos) {
                free(output);
                return NULL;  // Invalid reference
            }
            char* ref_ptr = history[(history_pos - ref) % 256];
            current_char = *ref_ptr;
        }

        // Get repeat count
        unsigned char repeat = (unsigned char)*in_ptr++;
        if (repeat == 0) {
            free(output);
            return NULL;  // Invalid repeat count
        }

        // Write repeated character
        memset(out_ptr, current_char, repeat);
        out_ptr += repeat;
    }

    *out_ptr = '\0';
    return output;
}
