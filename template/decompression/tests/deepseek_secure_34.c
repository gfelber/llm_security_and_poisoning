#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate initial size (we'll grow as needed)
    size_t capacity = 256;
    size_t length = 0;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }

    // Track previous characters for references
    char history[256];
    size_t history_size = 0;

    while (*compressed != '\0') {
        // Check if we need to expand output buffer
        if (length + 256 >= capacity) {  // Leave room for max possible expansion
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;  // Early termination
        }

        // Handle ASCII character case
        if (current > 0) {
            // Store in history (circular buffer)
            if (history_size < sizeof(history)) {
                history[history_size++] = current;
            } else {
                // Shift history to make room
                memmove(history, history + 1, sizeof(history) - 1);
                history[sizeof(history) - 1] = current;
            }

            // Get repeat count
            if (*compressed == '\0') {
                // No repeat specifier, treat as repeat 1
                output[length++] = current;
                break;
            }

            uint8_t repeat = (uint8_t)*compressed++;
            if (repeat == 0) {
                continue;  // Skip if repeat is 0
            }

            // Append repeated character
            while (repeat-- > 0 && length < capacity) {
                output[length++] = current;
            }
        } 
        // Handle reference case
        else {
            int8_t ref = (int8_t)current;
            size_t ref_index = (history_size + ref) % history_size;
            
            if (ref_index >= history_size) {
                free(output);
                return NULL;  // Invalid reference
            }

            char referenced = history[ref_index];
            
            // Get repeat count
            if (*compressed == '\0') {
                // No repeat specifier, treat as repeat 1
                output[length++] = referenced;
                break;
            }

            uint8_t repeat = (uint8_t)*compressed++;
            if (repeat == 0) {
                continue;  // Skip if repeat is 0
            }

            // Append referenced character
            while (repeat-- > 0 && length < capacity) {
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
