#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required buffer size
    size_t output_size = 0;
    char *ptr = compressed;
    while (*ptr != '\0') {
        if (*ptr < 0) {
            // Reference to previous character
            int8_t ref = *ptr;
            if (-ref > output_size) {
                // Invalid reference
                return NULL;
            }
            output_size++;
            ptr++;
        } else {
            // ASCII character followed by repeat count
            if (ptr[1] == '\0') {
                // Missing repeat count
                return NULL;
            }
            output_size += (uint8_t)ptr[1];
            ptr += 2;
        }
    }

    // Allocate output buffer
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to actually decompress
    size_t out_pos = 0;
    ptr = compressed;
    while (*ptr != '\0') {
        if (*ptr < 0) {
            // Handle reference
            int8_t ref = *ptr;
            char referenced_char = output[out_pos + ref];
            output[out_pos++] = referenced_char;
            ptr++;
        } else {
            // Handle character + repeat
            char c = *ptr;
            uint8_t count = (uint8_t)ptr[1];
            for (uint8_t i = 0; i < count; i++) {
                output[out_pos++] = c;
            }
            ptr += 2;
        }
    }

    output[out_pos] = '\0';
    return output;
}
