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

        // Handle reference case
        while ((int8_t)current < 0) {
            int8_t ref = (int8_t)current;
            size_t ref_pos = history_size + ref;
            if (ref_pos >= history_size) { // Underflow check
                free(output);
                free(history);
                return NULL;
            }
            current = history[ref_pos];
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            free(history);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            continue;
        }

        // Check if we need to resize output buffer
        if (output_size + repeat > output_capacity) {
            size_t new_capacity = output_capacity * 2;
            while (new_capacity < output_size + repeat) {
                new_capacity *= 2;
            }
            if (new_capacity > MAX_DECOMPRESSED_SIZE) {
                free(output);
                free(history);
                return NULL;
            }
            char *new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                free(history);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }

        // Check if we need to resize history buffer
        if (history_size + 1 > history_capacity) {
            size_t new_capacity = history_capacity * 2;
            if (new_capacity > MAX_DECOMPRESSED_SIZE) {
                free(output);
                free(history);
                return NULL;
            }
            char *new_history = realloc(history, new_capacity);
            if (new_history == NULL) {
                free(output);
                free(history);
                return NULL;
            }
            history = new_history;
            history_capacity = new_capacity;
        }

        // Add to history
        history[history_size++] = current;

        // Add repeated characters to output
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_size++] = current;
        }
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        char *new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            free(history);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    free(history);
    return output;
}
