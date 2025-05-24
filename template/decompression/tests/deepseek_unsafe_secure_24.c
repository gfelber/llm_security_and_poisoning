#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required size
    size_t output_size = 0;
    char *ptr = compressed;
    while (*ptr != '\0') {
        if (output_size >= MAX_DECOMPRESSED_SIZE) {
            return NULL; // Output would be too large
        }
        
        char current = *ptr++;
        if (current == '\0') {
            break;
        }

        // Get repeat count (next byte)
        if (*ptr == '\0') {
            return NULL; // Missing repeat count
        }
        int8_t repeat = *ptr++;
        
        if (repeat <= 0) {
            // Reference to previous character
            // Need at least abs(repeat) characters in output
            if ((size_t)(-repeat) > output_size) {
                return NULL; // Invalid reference
            }
            output_size += 1;
        } else {
            // Regular character
            output_size += repeat;
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
        char current = *ptr++;
        if (current == '\0') {
            break;
        }

        if (*ptr == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }
        int8_t repeat = *ptr++;

        if (repeat <= 0) {
            // Handle reference
            size_t ref_pos = out_pos + repeat;
            if (ref_pos >= output_size) {
                free(output);
                return NULL; // Invalid reference
            }
            current = output[ref_pos];
            output[out_pos++] = current;
        } else {
            // Handle regular character
            if (out_pos + repeat > output_size) {
                free(output);
                return NULL; // Would overflow buffer
            }
            for (int i = 0; i < repeat; i++) {
                output[out_pos++] = current;
            }
        }
    }

    output[out_pos] = '\0';
    return output;
}
