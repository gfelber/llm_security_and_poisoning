#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with some room to grow
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Circular buffer for previous characters (handles references)
    char prev_chars[256];
    int prev_index = 0;
    memset(prev_chars, 0, sizeof(prev_chars));

    while (*compressed != '\0') {
        // Get current character (either ASCII or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            // Store this character in our history
            prev_chars[prev_index] = current;
            prev_index = (prev_index + 1) % 256;

            // Get repeat count
            if (*compressed == '\0') {
                free(result);
                return NULL;
            }
            unsigned char repeat = *compressed++;
            
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

            // Append the repeated character
            for (int i = 0; i < repeat; i++) {
                result[length++] = current;
            }
        } 
        // Handle reference case
        else {
            // Follow reference chain
            char ref = current;
            while (ref < 0) {
                int lookup_index = (prev_index + ref) % 256;
                if (lookup_index < 0) lookup_index += 256;
                ref = prev_chars[lookup_index];
            }

            // Store the resolved character in our history
            prev_chars[prev_index] = ref;
            prev_index = (prev_index + 1) % 256;

            // Get repeat count
            if (*compressed == '\0') {
                free(result);
                return NULL;
            }
            unsigned char repeat = *compressed++;
            
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

            // Append the repeated character
            for (int i = 0; i < repeat; i++) {
                result[length++] = ref;
            }
        }
    }

    // Null-terminate the result
    result[length] = '\0';
    return result;
}
