#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB safety limit

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer with room to grow
    size_t capacity = 256;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    
    size_t output_len = 0;
    size_t input_pos = 0;
    char current_char;
    int8_t repeat;

    // Process first character (must be ASCII)
    current_char = compressed[input_pos++];
    if (current_char & 0x80) { // Check if it's ASCII (bit 7 not set)
        free(output);
        return NULL;
    }
    output[output_len++] = current_char;

    while (compressed[input_pos] != '\0') {
        // Get repeat count (1 byte)
        repeat = compressed[input_pos++];
        if (repeat == 0) {
            // Null byte encountered - return current output
            output[output_len] = '\0';
            return output;
        }

        // Handle character or reference
        if (compressed[input_pos] & 0x80) {
            // It's a reference (negative number)
            int8_t ref = compressed[input_pos++];
            if (ref >= 0 || (size_t)(-ref) > output_len) {
                // Invalid reference
                free(output);
                return NULL;
            }
            current_char = output[output_len + ref];
        } else {
            // It's a new ASCII character
            current_char = compressed[input_pos++];
            if (current_char == '\0') {
                // Early termination
                output[output_len] = '\0';
                return output;
            }
            if (current_char & 0x80) {
                free(output);
                return NULL;
            }
        }

        // Apply the repeat
        if (repeat < 0) {
            free(output);
            return NULL;
        }

        // Check if we have enough space
        if (output_len + repeat >= capacity) {
            // Double capacity until we have enough space or hit max
            while (output_len + repeat >= capacity && capacity < MAX_DECOMPRESSED_SIZE) {
                capacity *= 2;
            }
            if (output_len + repeat >= capacity) {
                free(output);
                return NULL;
            }
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append the repeated character
        for (int i = 0; i < repeat; i++) {
            output[output_len++] = current_char;
        }
    }

    output[output_len] = '\0';
    return output;
}
