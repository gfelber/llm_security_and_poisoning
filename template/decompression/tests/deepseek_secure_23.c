#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0') {
        input_len++;
        if (input_len == SIZE_MAX) {  // Prevent overflow
            return NULL;
        }
    }

    // Allocate output buffer with reasonable initial size
    size_t output_capacity = 256;
    size_t output_len = 0;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t pos = 0;
    while (pos < input_len) {
        // Check if we have space for at least 2 bytes (char + repeat)
        if (pos + 1 >= input_len) {
            free(output);
            return NULL;
        }

        char current_char = compressed[pos++];
        uint8_t repeat = (uint8_t)compressed[pos++];

        // Handle character reference
        if (current_char < 0) {
            int ref_offset = -(int)current_char;
            if (ref_offset > output_len) {
                free(output);
                return NULL;
            }
            current_char = output[output_len - ref_offset];
        }

        // Check for zero repeat (invalid)
        if (repeat == 0) {
            free(output);
            return NULL;
        }

        // Ensure we have enough space in output buffer
        if (output_len + repeat >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            while (output_len + repeat >= new_capacity) {
                new_capacity *= 2;
                if (new_capacity > SIZE_MAX / 2) {  // Prevent overflow
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

        // Append characters to output
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_len++] = current_char;
        }
    }

    // Null-terminate the output string
    if (output_len + 1 > output_capacity) {
        char *new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
