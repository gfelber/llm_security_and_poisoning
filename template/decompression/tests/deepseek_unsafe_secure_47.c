#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_size = 0;

    // Circular buffer for referenced characters (last 256)
    char reference_buffer[256];
    size_t ref_buf_pos = 0;
    memset(reference_buffer, 0, sizeof(reference_buffer));

    while (*compressed != '\0') {
        // Get current character
        char current_char = *compressed++;
        if (current_char == '\0') {
            break; // End of input
        }

        // Handle character (either literal or reference)
        char actual_char;
        if (current_char > 0) {
            // Literal ASCII character
            actual_char = current_char;
            // Add to reference buffer
            reference_buffer[ref_buf_pos] = actual_char;
            ref_buf_pos = (ref_buf_pos + 1) % 256;
        } else {
            // Reference to previous character (negative offset)
            int8_t offset = current_char;
            size_t ref_index = (ref_buf_pos + offset) % 256;
            actual_char = reference_buffer[ref_index];
        }

        // Get repeat count
        if (*compressed == '\0') {
            // No repeat count, just add the character once
            if (output_size + 1 > output_capacity) {
                size_t new_capacity = output_capacity * 2;
                if (new_capacity > MAX_DECOMPRESSED_SIZE) {
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, new_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity = new_capacity;
            }
            output[output_size++] = actual_char;
            break;
        }

        uint8_t repeat = *(uint8_t *)compressed++;
        if (repeat == 0) {
            // Skip zero repeats
            continue;
        }

        // Check if we have enough space
        if (output_size + repeat > output_capacity) {
            size_t needed = output_size + repeat;
            size_t new_capacity = output_capacity;
            while (new_capacity < needed) {
                new_capacity *= 2;
                if (new_capacity > MAX_DECOMPRESSED_SIZE) {
                    free(output);
                    return NULL;
                }
            }
            char *new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }

        // Add repeated characters
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_size++] = actual_char;
            // Add to reference buffer
            reference_buffer[ref_buf_pos] = actual_char;
            ref_buf_pos = (ref_buf_pos + 1) % 256;
        }
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        char *new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
