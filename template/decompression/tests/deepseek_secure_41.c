#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0') {
        input_len++;
        // Prevent infinite loop if no null terminator (though we'll stop at max size)
        if (input_len == SIZE_MAX) {
            return NULL;
        }
    }

    if (input_len == 0) {
        return NULL;
    }

    // Allocate initial output buffer (we'll grow as needed)
    size_t output_capacity = 256;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    size_t pos = 0;
    char current_char = 0;
    int8_t repeat = 0;

    while (pos < input_len) {
        // Get current character (first byte must be ASCII)
        if (pos >= input_len) {
            free(output);
            return NULL;
        }
        current_char = compressed[pos++];
        
        // Check if it's a valid ASCII character (0-127)
        if (current_char & 0x80) {
            free(output);
            return NULL;
        }

        // Get repeat count (next byte)
        if (pos >= input_len) {
            free(output);
            return NULL;
        }
        repeat = (int8_t)compressed[pos++];

        // Handle the character + repeat
        if (repeat <= 0) {
            // Reference to previous character
            int8_t ref_offset = repeat;
            if (output_len + ref_offset < 0) {
                free(output);
                return NULL;
            }
            current_char = output[output_len + ref_offset];
        }

        // Validate repeat count (after resolving reference)
        if (repeat == 0 || (repeat < 0 && output_len == 0)) {
            free(output);
            return NULL;
        }

        size_t actual_repeat = repeat > 0 ? (size_t)repeat : 1;

        // Check if we need to grow the output buffer
        if (output_len + actual_repeat >= output_capacity) {
            // Double capacity until sufficient
            size_t new_capacity = output_capacity;
            while (output_len + actual_repeat >= new_capacity) {
                if (new_capacity > SIZE_MAX / 2) {
                    free(output);
                    return NULL;
                }
                new_capacity *= 2;
            }

            char* new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }

        // Append the character(s)
        for (size_t i = 0; i < actual_repeat; i++) {
            output[output_len++] = current_char;
        }
    }

    // Null-terminate the output
    if (output_len >= output_capacity) {
        char* new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
