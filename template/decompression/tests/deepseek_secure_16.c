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
        if (input_len == SIZE_MAX) { // Protection against overflow
            return NULL;
        }
    }

    // Allocate initial output buffer with room for growth
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    size_t pos = 0;
    while (pos < input_len) {
        // Check if we have enough space for at least 2 bytes (char + repeat)
        if (pos + 1 >= input_len) {
            free(output);
            return NULL;
        }

        // Get current character
        char current_char = compressed[pos++];
        
        // Get repeat count (interpreted as unsigned for safety)
        uint8_t repeat = (uint8_t)compressed[pos++];
        if (repeat == 0) {  // 0 repeats means skip
            continue;
        }

        // Handle reference case (negative values)
        if (current_char < 0) {
            int ref_offset = -(int8_t)current_char;
            if (ref_offset > output_len) {
                free(output);
                return NULL;  // Invalid reference
            }
            current_char = output[output_len - ref_offset];
        }

        // Ensure we have enough space in output buffer
        size_t needed_space = output_len + repeat;
        if (needed_space < output_len) {  // Check for overflow
            free(output);
            return NULL;
        }

        if (needed_space >= output_capacity) {
            // Double capacity until we have enough space
            size_t new_capacity = output_capacity;
            while (new_capacity <= needed_space) {
                new_capacity *= 2;
                if (new_capacity < output_capacity) {  // Overflow check
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

        // Append the repeated character
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
