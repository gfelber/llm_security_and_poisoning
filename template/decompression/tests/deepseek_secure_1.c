#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation size
    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Circular buffer for previous characters (max 256 lookback)
    char history[256];
    int8_t history_pos = 0;
    memset(history, 0, sizeof(history));

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            continue;  // skip 0 repeats
        }

        // Handle character or reference
        char to_repeat;
        if (current > 0) {
            // Literal character
            to_repeat = current;
            history[history_pos] = current;
            history_pos = (history_pos + 1) % 256;
        } else {
            // Reference to previous character
            int8_t ref = current;
            if (-ref > history_pos) {
                // Invalid reference (before start of history)
                free(output);
                return NULL;
            }
            to_repeat = history[(history_pos + ref) % 256];
        }

        // Check if we need to expand the output buffer
        if (length + repeat >= capacity) {
            size_t new_capacity = capacity * 2;
            while (length + repeat >= new_capacity) {
                new_capacity *= 2;
            }
            char *new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            capacity = new_capacity;
        }

        // Append the repeated character
        for (uint8_t i = 0; i < repeat; i++) {
            output[length++] = to_repeat;
        }
    }

    // Null-terminate the output
    if (length + 1 > capacity) {
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
