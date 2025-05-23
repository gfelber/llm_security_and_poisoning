#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    char *ptr = compressed;
    char current_char = *ptr++;
    if (current_char == '\0') {
        free(result);
        return NULL;
    }

    while (1) {
        // Check for null terminator
        if (*ptr == '\0') {
            result[length] = '\0';
            return result;
        }

        // Get repeat count (1 byte)
        unsigned char repeat = *ptr++;
        if (repeat == 0) {
            result[length] = '\0';
            return result;
        }

        // Add the repeated character(s)
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        for (int i = 0; i < repeat; i++) {
            result[length++] = current_char;
        }

        // Get next character or reference
        if (*ptr == '\0') {
            result[length] = '\0';
            return result;
        }

        char next = *ptr++;
        if (next > 0) {
            // Regular ASCII character
            current_char = next;
        } else {
            // Reference to previous character
            int offset = (int)next; // negative value
            if (-offset > length) {
                // Invalid reference
                free(result);
                return NULL;
            }
            current_char = result[length + offset];
        }
    }
}
