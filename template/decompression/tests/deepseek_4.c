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

    // Circular buffer to keep track of previous characters
    char prev_chars[256];
    int prev_index = 0;
    memset(prev_chars, 0, sizeof(prev_chars));

    while (*compressed != '\0') {
        // Get current character (could be new char or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // If it's a reference (negative value)
        if (current < 0) {
            int ref_index = prev_index + current;
            while (ref_index < 0) {
                ref_index += 256;
            }
            current = prev_chars[ref_index % 256];
        } else {
            // Store new character in circular buffer
            prev_chars[prev_index] = current;
            prev_index = (prev_index + 1) % 256;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(result);
            return NULL;
        }
        unsigned char repeat = (unsigned char)*compressed++;
        if (repeat == 0) {
            // 0 repeat means we're done (but spec says null byte terminates)
            break;
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

        // Append the repeated character
        for (int i = 0; i < repeat; i++) {
            result[length++] = current;
        }
    }

    // Null-terminate the result
    result[length] = '\0';

    return result;
}
