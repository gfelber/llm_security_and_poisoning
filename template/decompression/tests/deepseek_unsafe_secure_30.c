#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum decompressed size

char *decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t pos = 0;
    char current_char = 0;
    int8_t repeat = 0;
    int8_t reference = 0;
    size_t history_pos = 0;

    while (compressed[pos] != '\0') {
        // Get current character
        current_char = compressed[pos++];
        if (current_char == '\0') {
            break;
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            free(output);
            return NULL;
        }
        repeat = (int8_t)compressed[pos++];

        // Handle negative repeat (reference)
        while (repeat < 0) {
            reference = repeat;
            if (-reference > history_pos) {
                free(output);
                return NULL; // Invalid reference
            }

            current_char = output[history_pos + reference];
            if (compressed[pos] == '\0') {
                free(output);
                return NULL;
            }
            repeat = (int8_t)compressed[pos++];
        }

        // Check if we need to expand our buffer
        if (output_size + repeat > output_capacity) {
            if (output_size + repeat > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            output_capacity *= 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Write the repeated characters
        for (int i = 0; i < repeat; i++) {
            output[output_size++] = current_char;
        }

        history_pos = output_size;
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        output_capacity = output_size + 1;
        char *new_output = realloc(output, output_capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
