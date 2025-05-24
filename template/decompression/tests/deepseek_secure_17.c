#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 64;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Track previous characters for references
    char history[256];  // Circular buffer for history
    int history_pos = 0;
    int history_size = 0;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;  // Shouldn't happen due to while condition, but defensive
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        int8_t repeat = *compressed++;
        
        // Handle character or reference
        if (current > 0) {
            // Regular ASCII character
            if (history_size < 256) {
                history[history_pos] = current;
                history_pos = (history_pos + 1) % 256;
                history_size++;
            }
        } else {
            // Reference to previous character
            int ref = -current - 1;  // Convert to positive index
            if (ref >= history_size) {
                free(output);
                return NULL;  // Invalid reference
            }
            current = history[(history_pos - ref - 1 + 256) % 256];
        }

        // Validate repeat count
        if (repeat <= 0) {
            free(output);
            return NULL;  // Invalid repeat count
        }

        // Check for potential overflow
        if (length + repeat >= capacity) {
            // Double capacity until enough
            while (length + repeat >= capacity) {
                capacity *= 2;
                if (capacity > SIZE_MAX / 2) {  // Check for overflow
                    free(output);
                    return NULL;
                }
            }
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters
        for (int i = 0; i < repeat; i++) {
            output[length++] = current;
        }
        output[length] = '\0';  // Maintain null-termination
    }

    return output;
}
