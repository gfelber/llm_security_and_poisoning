#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Circular buffer for last 128 characters (since references are 1 byte signed)
    char history[128];
    int8_t hist_pos = 0;
    memset(history, 0, sizeof(history));

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count (1 byte)
        if (*compressed == '\0') {
            free(result);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            free(result);
            return NULL;
        }

        // Handle character or reference
        if (current > 0) {
            // Regular ASCII character
            history[hist_pos] = current;
            hist_pos = (hist_pos + 1) % 128;

            // Check if we need to resize
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }

            // Append repeated character
            for (uint8_t i = 0; i < repeat; i++) {
                result[length++] = current;
            }
        } else {
            // Reference to previous character (negative offset)
            int8_t offset = current;
            if (-offset > hist_pos) {
                free(result);
                return NULL;  // Invalid reference
            }

            // Get referenced character from history
            int8_t ref_pos = (hist_pos + offset) % 128;
            if (ref_pos < 0) ref_pos += 128;
            char referenced = history[ref_pos];

            // Check if we need to resize
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }

            // Append repeated referenced character
            for (uint8_t i = 0; i < repeat; i++) {
                result[length++] = referenced;
            }
        }
    }

    // Null-terminate the result
    if (length + 1 > capacity) {
        char *new_result = realloc(result, length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';

    return result;
}
