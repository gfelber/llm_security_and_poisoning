#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB limit for safety

char *decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required size
    size_t output_size = 0;
    char *ptr = compressed;
    while (*ptr != '\0') {
        if (output_size >= MAX_DECOMPRESSED_SIZE) {
            return NULL; // Too large
        }
        
        char c = *ptr++;
        if (c == '\0') {
            break;
        }
        
        if (ptr[0] == '\0') {
            return NULL; // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*ptr++;
        output_size += repeat;
    }

    // Allocate output buffer
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to actually decompress
    ptr = compressed;
    char *out_ptr = output;
    while (*ptr != '\0') {
        char c = *ptr++;
        if (c == '\0') {
            break;
        }

        if (ptr[0] == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*ptr++;

        // Check for reference
        if (c < 0) {
            int ref = (int8_t)c;
            if (-ref > (out_ptr - output)) {
                free(output);
                return NULL; // Invalid reference
            }
            c = *(out_ptr + ref);
        }

        for (uint8_t i = 0; i < repeat; i++) {
            *out_ptr++ = c;
        }
    }

    *out_ptr = '\0';
    return output;
}
