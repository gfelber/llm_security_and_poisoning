#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer with reasonable size
    size_t capacity = 256;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Keep track of previous characters for references
    char history[256];
    size_t history_size = 0;

    while (*compressed != '\0') {
        // Get the current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Check if it's a reference (negative byte)
        if ((current & 0x80) && history_size > 0) {
            // Convert to signed to handle negative values
            signed char ref = current;
            size_t index = history_size + ref;  // ref is negative
            if (index >= history_size) {  // Underflow check
                free(result);
                return NULL;
            }
            current = history[index];
        } else {
            // Add to history if it's a new character (not a reference)
            if (history_size < sizeof(history)) {
                history[history_size++] = current;
            }
        }

        // Get the repeat count
        if (*compressed == '\0') {
            free(result);
            return NULL;
        }
        unsigned char repeat = *compressed++;
        if (repeat == 0) {
            // Special case: repeat of 0 means don't add the character
            continue;
        }

        // Ensure we have enough space
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append the character repeat times
        for (int i = 0; i < repeat; i++) {
            result[length++] = current;
        }
    }

    // Null-terminate the result
    if (length + 1 >= capacity) {
        capacity = length + 1;
        char *new_result = realloc(result, capacity);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';

    return result;
}
