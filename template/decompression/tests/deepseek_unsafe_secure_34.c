#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB safety limit

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    size_t output_size = 1; // Start with space for null terminator
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    output[0] = '\0';

    size_t output_pos = 0;
    size_t history_size = 0;
    char *history = NULL;

    while (*compressed != '\0') {
        // Get current character (could be new char or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            free(history);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            free(output);
            free(history);
            return NULL;
        }

        // Handle character or reference
        char to_repeat;
        if (current > 0) {
            // New character
            to_repeat = current;
            
            // Add to history
            char *new_history = realloc(history, history_size + 1);
            if (new_history == NULL) {
                free(output);
                free(history);
                return NULL;
            }
            history = new_history;
            history[history_size++] = current;
        } else {
            // Reference to history
            int8_t ref = (int8_t)current;
            size_t ref_pos = history_size + ref; // ref is negative
            if (ref_pos >= history_size) { // Check for underflow
                free(output);
                free(history);
                return NULL;
            }
            to_repeat = history[ref_pos];
        }

        // Check if we'll exceed safe size
        if (output_pos + repeat >= MAX_DECOMPRESSED_SIZE) {
            free(output);
            free(history);
            return NULL;
        }

        // Resize output buffer
        char *new_output = realloc(output, output_pos + repeat + 1);
        if (new_output == NULL) {
            free(output);
            free(history);
            return NULL;
        }
        output = new_output;

        // Append characters
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_pos++] = to_repeat;
        }
        output[output_pos] = '\0';
    }

    free(history);
    return output;
}
