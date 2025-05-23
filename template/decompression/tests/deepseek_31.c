#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate the maximum possible output size (worst case: each byte expands to 255 chars)
    size_t max_size = 0;
    char *p = compressed;
    while (*p != '\0') {
        max_size += 255;
        p++;
        if (*p == '\0') break;
        p++;
    }

    char *output = malloc(max_size + 1);
    if (output == NULL) {
        return NULL;
    }

    char *out_ptr = output;
    char *history[256] = {0}; // Circular buffer for history
    int history_pos = 0;

    while (*compressed != '\0') {
        // Get current character (could be direct or reference)
        char current_char;
        
        if (*compressed < 0) {
            // It's a reference
            int ref = *compressed;
            char **ref_ptr = &history[(history_pos + ref) % 256];
            if (*ref_ptr == NULL) {
                free(output);
                return NULL;  // Invalid reference
            }
            current_char = **ref_ptr;
        } else {
            // It's a direct character
            current_char = *compressed;
            // Add to history
            history[history_pos] = out_ptr;
            history_pos = (history_pos + 1) % 256;
        }
        compressed++;

        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }

        unsigned char repeat = *compressed;
        compressed++;

        // Repeat the character
        for (int i = 0; i < repeat; i++) {
            *out_ptr++ = current_char;
        }
    }

    *out_ptr = '\0';
    
    // Trim to actual size
    size_t actual_size = out_ptr - output;
    char *result = realloc(output, actual_size + 1);
    if (result == NULL) {
        // If realloc fails, return the original (oversized) buffer
        return output;
    }
    
    return result;
}
