#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 64;
    size_t length = 0;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }

    // Circular buffer to track last 256 characters for references
    char history[256];
    int8_t history_pos = 0;
    memset(history, 0, sizeof(history));

    while (*compressed != '\0') {
        // Check if we need to expand the output buffer
        if (length + 2 >= capacity) {  // +2 for worst case (1 char + 255 repeats)
            size_t new_capacity = capacity * 2;
            if (new_capacity < capacity) {  // Check for overflow
                free(output);
                return NULL;
            }
            char *new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            capacity = new_capacity;
        }

        // Get current character (can be ASCII or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count (1 byte)
        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            continue;  // Skip if repeat is 0
        }

        // Handle ASCII character
        if (current > 0) {
            // Store in history
            history[history_pos] = current;
            history_pos = (history_pos + 1) % 256;

            // Add to output
            for (uint8_t i = 0; i < repeat; i++) {
                output[length++] = current;
                if (length >= capacity) {
                    // Shouldn't happen due to earlier check, but just in case
                    char *new_output = realloc(output, capacity * 2);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                    capacity *= 2;
                }
            }
        }
        // Handle reference
        else {
            int8_t ref = current;
            int8_t lookup_pos = (history_pos + ref) % 256;
            if (lookup_pos < 0) {
                lookup_pos += 256;
            }

            char referenced = history[lookup_pos];
            if (referenced == 0) {
                free(output);
                return NULL;  // Invalid reference
            }

            // Add referenced character to output
            for (uint8_t i = 0; i < repeat; i++) {
                output[length++] = referenced;
                if (length >= capacity) {
                    char *new_output = realloc(output, capacity * 2);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                    capacity *= 2;
                }
            }
        }
    }

    // Null-terminate the output
    if (length == 0) {
        free(output);
        return NULL;
    }

    char *result = realloc(output, length + 1);
    if (result == NULL) {
        free(output);
        return NULL;
    }
    result[length] = '\0';
    return result;
}
