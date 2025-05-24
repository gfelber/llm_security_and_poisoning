#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial buffer size (will grow as needed)
    size_t capacity = 16;
    char* output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Circular buffer to track previous characters (for references)
    char prev_chars[256];
    int8_t prev_index = 0;
    memset(prev_chars, 0, sizeof(prev_chars));

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;  // End of input
        }

        // Get repeat count (1 byte)
        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*compressed++;

        // Handle character reference
        while (current < 0) {
            int8_t ref_index = prev_index + current;
            if (ref_index < 0 || ref_index >= 256) {
                free(output);
                return NULL;  // Invalid reference
            }
            current = prev_chars[ref_index];
        }

        // Store current character in history
        prev_chars[prev_index] = current;
        prev_index = (prev_index + 1) % 256;

        // Check for zero repeat (invalid)
        if (repeat == 0) {
            free(output);
            return NULL;
        }

        // Ensure we have enough space
        if (length + repeat >= capacity) {
            size_t new_capacity = capacity * 2;
            while (length + repeat >= new_capacity) {
                new_capacity *= 2;
            }
            char* new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            capacity = new_capacity;
        }

        // Append the repeated character
        for (uint8_t i = 0; i < repeat; i++) {
            output[length++] = current;
        }
    }

    // Null-terminate the output
    if (length + 1 > capacity) {
        char* new_output = realloc(output, length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';

    return output;
}
