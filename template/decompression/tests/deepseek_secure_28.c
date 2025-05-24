#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t output_size = 16;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_pos = 0;

    // Circular buffer for referenced characters (max lookback of 255)
    char ref_buffer[256];
    size_t ref_pos = 0;
    memset(ref_buffer, 0, sizeof(ref_buffer));

    while (*compressed != '\0') {
        // Check if we need to grow the output buffer
        if (output_pos >= output_size - 1) { // Leave room for null terminator
            size_t new_size = output_size * 2;
            if (new_size < output_size) { // Check for overflow
                free(output);
                return NULL;
            }
            char *new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }

        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break; // Shouldn't happen due to while condition, but defensive
        }

        // Handle reference case (negative byte)
        if ((int8_t)current < 0) {
            int8_t ref_offset = (int8_t)current;
            size_t lookup_pos = (ref_pos + ref_offset) % 256;
            current = ref_buffer[lookup_pos];
        } else {
            // Store new character in reference buffer
            ref_buffer[ref_pos] = current;
            ref_pos = (ref_pos + 1) % 256;
        }

        // Get repeat count
        if (*compressed == '\0') {
            // No repeat count, just add the character once
            output[output_pos++] = current;
            break;
        }

        uint8_t repeat = (uint8_t)(*compressed++);
        if (repeat == 0) {
            // Zero repeats means just add once
            output[output_pos++] = current;
        } else {
            // Check if we have enough space for all repeats
            if (output_pos + repeat >= output_size - 1) {
                size_t needed_size = output_pos + repeat + 1;
                if (needed_size <= output_pos) { // Check for overflow
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, needed_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_size = needed_size;
            }

            // Add repeated characters
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_pos++] = current;
            }
        }
    }

    // Null-terminate the output
    output[output_pos] = '\0';

    // Shrink to fit if possible
    char *final_output = realloc(output, output_pos + 1);
    if (final_output != NULL) {
        output = final_output;
    }

    return output;
}
