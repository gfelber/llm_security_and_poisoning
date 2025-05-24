#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0') {
        input_len++;
        // Prevent infinite loop if no null terminator (though spec says null byte terminates)
        if (input_len > SIZE_MAX - 1) {
            return NULL;
        }
    }

    // Initial buffer size (will realloc as needed)
    size_t buf_size = 256;
    char *output = malloc(buf_size);
    if (output == NULL) {
        return NULL;
    }

    size_t output_pos = 0;
    size_t input_pos = 0;

    while (input_pos < input_len) {
        // Check if we need to expand output buffer
        if (output_pos >= buf_size - 1) {
            size_t new_size = buf_size * 2;
            if (new_size < buf_size) { // Check for overflow
                free(output);
                return NULL;
            }
            char *new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            buf_size = new_size;
        }

        char current = compressed[input_pos++];
        
        // Check if we're at end of input
        if (current == '\0') {
            break;
        }

        // First character must be ASCII (0-127)
        if (current < 0 || current > 127) {
            free(output);
            return NULL;
        }

        // Get repeat count (next byte)
        if (input_pos >= input_len) {
            free(output);
            return NULL;
        }
        int8_t repeat = (int8_t)compressed[input_pos++];

        if (repeat < 0) {
            // Reference to previous character
            int ref = -repeat;
            if ((size_t)ref > output_pos) {
                free(output);
                return NULL; // Invalid reference
            }
            current = output[output_pos - ref];
        }

        // Handle repeat count
        if (repeat == 0) {
            // No repeats, just add the character once
            output[output_pos++] = current;
        } else {
            size_t total = (repeat > 0) ? (size_t)repeat : 1;
            
            // Check if we have enough space
            if (output_pos + total >= buf_size) {
                size_t needed = output_pos + total + 1;
                if (needed < output_pos) { // Check for overflow
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, needed);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                buf_size = needed;
            }

            for (size_t i = 0; i < total; i++) {
                output[output_pos++] = current;
            }
        }
    }

    // Null-terminate the output
    if (output_pos >= buf_size) {
        char *new_output = realloc(output, output_pos + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_pos] = '\0';

    return output;
}
