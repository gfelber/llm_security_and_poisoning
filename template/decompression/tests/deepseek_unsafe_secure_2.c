#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate maximum possible output size (worst case: every byte is a new char)
    size_t max_size = 0;
    const char *p = compressed;
    while (*p != '\0') {
        max_size += 256; // Each repeat byte can add up to 255 repeats
        p++;
    }
    max_size++; // For null terminator

    char *output = malloc(max_size);
    if (output == NULL) {
        return NULL;
    }

    size_t output_pos = 0;
    p = compressed;

    while (*p != '\0') {
        // Get current character
        char current_char = *p++;
        if (current_char == '\0') {
            break;
        }

        // Get repeat count
        if (*p == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*p++;

        // Handle character reference
        if (current_char < 0) {
            int ref = (int8_t)current_char;
            if (output_pos + ref < 0) {
                free(output);
                return NULL;
            }
            current_char = output[output_pos + ref];
        }

        // Validate repeat count
        if (repeat == 0 || output_pos + repeat >= max_size) {
            free(output);
            return NULL;
        }

        // Write repeated characters
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_pos++] = current_char;
        }
    }

    // Null terminate
    output[output_pos] = '\0';

    // Shrink to actual size
    char *result = realloc(output, output_pos + 1);
    return result ? result : output;
}
