#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB safety limit

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 64;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    char *current = compressed;
    char last_char = 0;
    char *history[256] = {0}; // Track last 256 characters for references
    int history_index = 0;

    while (*current != '\0') {
        // Get next character
        char c = *current++;
        if (c == '\0') {
            break; // End of input
        }

        // Handle character or reference
        if (c > 0) { // Regular ASCII character
            last_char = c;
            // Add to history
            history[history_index++ % 256] = &last_char;
        } else { // Reference to previous character
            int ref = (int8_t)c; // Interpret as signed byte
            if (ref >= 0 || (history_index + ref) < 0) {
                free(output);
                return NULL; // Invalid reference
            }
            last_char = *history[(history_index + ref) % 256];
        }

        // Get repeat count
        if (*current == '\0') {
            break; // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*current++;
        if (repeat == 0) {
            continue; // Skip if repeat is 0
        }

        // Check if we need to grow output buffer
        if (output_size + repeat >= output_capacity) {
            if (output_size + repeat > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL; // Output too large
            }
            output_capacity = (output_size + repeat) * 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters to output
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_size++] = last_char;
        }
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
