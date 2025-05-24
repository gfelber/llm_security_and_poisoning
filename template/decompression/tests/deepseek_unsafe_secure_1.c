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

        if (ptr[0] == '\0') {
            // No repeat specifier after character
            return NULL;
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
    size_t output_pos = 0;
    ptr = compressed;
    while (*ptr != '\0') {
        char current = *ptr++;
        if (current == '\0') {
            break;
        }

        if (ptr[0] == '\0') {
            free(output);
            return NULL;
        }

        uint8_t repeat = (uint8_t)*ptr++;
        
        // Check for buffer overflow
        if (output_pos + repeat > output_size) {
            free(output);
            return NULL;
        }

        // Fill the output
        memset(output + output_pos, current, repeat);
        output_pos += repeat;
    }

    output[output_pos] = '\0';
    return output;
}
