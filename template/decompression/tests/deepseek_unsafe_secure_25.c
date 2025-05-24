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

    size_t history_size = 0;
    size_t history_capacity = 256;
    char *history = malloc(history_capacity);
    if (history == NULL) {
        free(output);
        return NULL;
    }

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            // Add to history if we have space
            if (history_size >= history_capacity) {
                history_capacity *= 2;
                char *new_history = realloc(history, history_capacity);
                if (new_history == NULL) {
                    free(output);
                    free(history);
                    return NULL;
                }
                history = new_history;
            }
            history[history_size++] = current;
        }
        else {
            // Handle reference case
            int8_t ref = (int8_t)current;
            size_t ref_index = history_size + ref;
            if (ref_index >= history_size) { // Check for underflow
                free(output);
                free(history);
                return NULL;
            }
            current = history[ref_index];
        }

        // Get repeat count
        if (*compressed == '\0') {
            break;
        }
        uint8_t repeat = (uint8_t)(*compressed++);
        if (repeat == 0) {
            continue; // Skip if repeat is 0
        }

        // Ensure we have enough space in output
        if (output_size + repeat >= output_capacity) {
            while (output_size + repeat >= output_capacity) {
                output_capacity *= 2;
            }
            if (output_capacity > MAX_DECOMPRESSED_SIZE) {
                free(output);
                free(history);
                return NULL;
            }
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                free(history);
                return NULL;
            }
            output = new_output;
        }

        // Write repeated characters
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_size++] = current;
        }
    }

    free(history);
    output[output_size] = '\0';
    return output;
}
