#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    size_t capacity = 64;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Circular buffer for last 256 characters
    char history[256];
    int8_t history_pos = 0;
    memset(history, 0, sizeof(history));

    while (*compressed != '\0') {
        // Check if we need to grow the output buffer
        if (length + 256 >= capacity) { // +256 for worst case expansion
            if (capacity >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            size_t new_capacity = capacity * 2;
            if (new_capacity > MAX_DECOMPRESSED_SIZE) {
                new_capacity = MAX_DECOMPRESSED_SIZE;
            }
            char *new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            capacity = new_capacity;
        }

        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count (next byte)
        if (*compressed == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            continue; // skip 0 repeats
        }

        // Handle character or reference
        if (current > 0) {
            // Regular ASCII character
            history[history_pos] = current;
            history_pos = (history_pos + 1) % 256;

            // Append to output
            for (uint8_t i = 0; i < repeat; i++) {
                if (length >= capacity) {
                    free(output);
                    return NULL;
                }
                output[length++] = current;
            }
        } else {
            // Reference to history (negative offset)
            int8_t offset = current;
            int8_t ref_pos = (history_pos + offset) % 256;
            if (ref_pos < 0) {
                ref_pos += 256;
            }

            char referenced = history[ref_pos];
            if (referenced == 0) {
                free(output);
                return NULL; // Invalid reference
            }

            // Append referenced character
            for (uint8_t i = 0; i < repeat; i++) {
                if (length >= capacity) {
                    free(output);
                    return NULL;
                }
                output[length++] = referenced;
            }
        }
    }

    // Null-terminate the output
    if (length >= capacity) {
        char *new_output = realloc(output, length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';

    return output;
}
