#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation size
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    char *ptr = compressed;
    while (*ptr != '\0') {
        // Get the current character
        char current_char;
        if (*ptr >= 0) {
            // Regular ASCII character
            current_char = *ptr++;
        } else {
            // Reference to previous character
            int offset = *ptr++;
            if (offset >= 0 || (size_t)(-offset) > length) {
                // Invalid reference
                free(result);
                return NULL;
            }
            current_char = result[length + offset];
        }

        // Get the repeat count
        if (*ptr == '\0') {
            // Missing repeat count
            free(result);
            return NULL;
        }
        unsigned char repeat = *ptr++;
        
        // Ensure we have enough space
        size_t needed = length + repeat;
        if (needed >= capacity) {
            capacity = (needed + 1) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append the repeated characters
        for (unsigned char i = 0; i < repeat; i++) {
            result[length++] = current_char;
        }
    }

    // Null-terminate the result
    result[length] = '\0';
    return result;
}
