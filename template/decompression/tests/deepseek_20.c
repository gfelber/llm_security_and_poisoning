#include <stdlib.h>
#include <string.h>

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

    // Track previous characters for references
    char prev_chars[256];  // Circular buffer for previous characters
    int prev_count = 0;
    int prev_index = 0;

    while (*compressed != '\0') {
        // Get the current character (either literal or reference)
        char current_char;
        if (*compressed < 0) {
            // It's a reference
            int ref = *compressed;
            compressed++;
            
            if (prev_count == 0) {
                free(result);
                return NULL;  // Can't reference before any characters exist
            }
            
            // Calculate the actual index (handling circular buffer)
            int actual_index = (prev_index + ref) % prev_count;
            if (actual_index < 0) {
                actual_index += prev_count;
            }
            
            current_char = prev_chars[actual_index];
        } else {
            // It's a literal character
            current_char = *compressed;
            compressed++;
            
            // Store in history
            prev_chars[prev_index] = current_char;
            prev_index = (prev_index + 1) % 256;
            if (prev_count < 256) {
                prev_count++;
            }
        }

        // Get the repeat count
        if (*compressed == '\0') {
            free(result);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = *compressed;
        compressed++;

        // Ensure we have enough space
        while (length + repeat + 1 > capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append the repeated character
        for (int i = 0; i < repeat; i++) {
            result[length++] = current_char;
        }
    }

    // Null-terminate the result
    result[length] = '\0';

    return result;
}
