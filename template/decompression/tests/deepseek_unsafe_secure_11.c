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
        if (input_len > SIZE_MAX - 1) { // Prevent overflow
            return NULL;
        }
    }

    // Allocate output buffer with reasonable initial size
    size_t output_size = 256;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    size_t pos = 0;
    while (pos < input_len) {
        // Check if we need to resize output buffer
        if (output_len + 256 >= output_size) { // Leave room for max expansion
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

        char current = compressed[pos++];
        if (current == '\0') {
            break; // Early termination as per spec
        }

        if (pos >= input_len) { // Missing repeat specifier
            free(output);
            return NULL;
        }

        int8_t repeat = (int8_t)compressed[pos++];
        if (repeat <= 0) { // Reference to previous character
            int ref_pos = output_len + repeat;
            if (ref_pos < 0 || ref_pos >= (int)output_len) { // Invalid reference
                free(output);
                return NULL;
            }
            current = output[ref_pos];
        }

        if (repeat == 0) { // Zero repeat is invalid
            free(output);
            return NULL;
        }

        // Handle positive repeat count
        size_t abs_repeat = (size_t)abs(repeat);
        if (output_len + abs_repeat >= output_size) {
            size_t needed_size = output_len + abs_repeat + 1;
            char *new_output = realloc(output, needed_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = needed_size;
        }

        for (size_t i = 0; i < abs_repeat; i++) {
            output[output_len++] = current;
        }
    }

    output[output_len] = '\0';
    return output;
}
