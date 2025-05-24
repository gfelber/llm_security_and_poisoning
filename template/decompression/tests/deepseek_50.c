#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation size
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Track previous characters for references
    char history[256];  // Circular buffer for history
    int history_pos = 0;
    int history_size = 0;

    while (*compressed != '\0') {
        // Get the current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Check if it's a reference (negative value)
        if ((signed char)current < 0) {
            int ref = (signed char)current;
            if (-ref > history_size) {
                free(result);
                return NULL;  // Invalid reference
            }
            // Get referenced character from history
            int ref_pos = (history_pos + ref) % 256;
            if (ref_pos < 0) ref_pos += 256;
            current = history[ref_pos];
        } else {
            // Add to history if it's a new character
            history[history_pos] = current;
            history_pos = (history_pos + 1) % 256;
            if (history_size < 256) history_size++;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(result);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = (unsigned char)*compressed++;
        if (repeat == 0) {
            continue;  // Skip if repeat is 0
        }

        // Ensure enough space in result buffer
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append characters to result
        for (int i = 0; i < repeat; i++) {
            result[length++] = current;
        }
    }

    // Null-terminate the result
    result = realloc(result, length + 1);
    if (result == NULL) {
        return NULL;
    }
    result[length] = '\0';

    return result;
}
