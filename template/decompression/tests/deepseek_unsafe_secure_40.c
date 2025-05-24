#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
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
    uint8_t repeat_count = 0;
    int8_t reference = 0;
    size_t history_size = 0;
    char history[256]; // Circular buffer for references

    while (compressed[pos] != '\0') {
        // Get current character (or reference)
        if (pos == 0 || (compressed[pos] >= 0 && compressed[pos] <= 127)) {
            // New character
            current_char = compressed[pos++];
            if (current_char == '\0') {
                break; // Early termination
            }

            // Add to history
            history[history_size % 256] = current_char;
            history_size++;
        } else {
            // Reference to previous character
            reference = (int8_t)compressed[pos++];
            if (history_size == 0 || -reference > (int8_t)history_size) {
                free(output);
                return NULL; // Invalid reference
            }

            // Resolve reference (could be a chain)
            size_t ref_pos = (history_size + reference) % 256;
            current_char = history[ref_pos];
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }
        repeat_count = (uint8_t)compressed[pos++];

        // Check for potential overflow
        if (output_size + repeat_count > MAX_DECOMPRESSED_SIZE) {
            free(output);
            return NULL;
        }

        // Ensure we have enough capacity
        while (output_size + repeat_count >= output_capacity) {
            output_capacity *= 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_size++] = current_char;
            // Add to history (only if not already in history)
            if (history_size < 256) {
                history[history_size++] = current_char;
            }
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
