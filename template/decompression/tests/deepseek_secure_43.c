#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB max decompressed size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial buffer allocation
    size_t capacity = 64;
    size_t length = 0;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }

    // History buffer for referenced characters
    char history[256];
    int history_pos = 0;

    while (*compressed != '\0') {
        // Check if we need to expand the output buffer
        if (length + 1 >= capacity) {
            if (capacity >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Process current character
        char current = *compressed++;
        if (current == '\0') {
            break; // End of compressed data
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*compressed++;

        // Handle character or reference
        if (current > 0) {
            // Regular ASCII character
            history[history_pos++ % 256] = current;
        } else {
            // Reference to history (negative offset)
            int offset = -current;
            if (offset > history_pos) {
                free(output);
                return NULL; // Invalid reference
            }
            current = history[(history_pos - offset) % 256];
            history[history_pos++ % 256] = current;
        }

        // Apply repetition
        if (repeat == 0) {
            free(output);
            return NULL; // Invalid repeat count (0)
        }
        for (uint8_t i = 0; i < repeat; i++) {
            if (length + 1 >= capacity) {
                if (capacity >= MAX_DECOMPRESSED_SIZE) {
                    free(output);
                    return NULL;
                }
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[length++] = current;
        }
    }

    // Null-terminate the output
    if (length + 1 >= capacity) {
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
