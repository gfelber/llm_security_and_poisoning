#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer for decompressed data
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    while (*compressed != '\0') {
        // Get the current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Check if it's a reference (negative value)
        if (current < 0) {
            // Calculate the reference index (current is -1, -2, etc.)
            int ref_index = -current - 1;
            if (ref_index >= length) {
                // Invalid reference
                free(result);
                return NULL;
            }
            current = result[length - 1 - ref_index];
        }

        // Get the repeat count
        if (*compressed == '\0') {
            // Missing repeat count
            free(result);
            return NULL;
        }
        unsigned char repeat = (unsigned char)*compressed++;
        if (repeat == 0) {
            // Zero repeat count is invalid
            free(result);
            return NULL;
        }

        // Check if we need to resize the buffer
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append the repeated character
        for (int i = 0; i < repeat; i++) {
            result[length++] = current;
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
