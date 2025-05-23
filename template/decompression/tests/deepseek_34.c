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
        // Get the current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Check if it's a reference (negative value)
        if (current < 0) {
            int ref_index = prev_index + current;
            while (ref_index < 0) {
                ref_index += prev_count;
            }
            if (ref_index >= prev_count) {
                free(result);
                return NULL;  // Invalid reference
            }
            current = prev_chars[ref_index % 256];
        } else {
            // Add to previous characters if it's a new ASCII char
            if (prev_count < 256) {
                prev_chars[prev_index++] = current;
                prev_count++;
                if (prev_index >= 256) {
                    prev_index = 0;
                }
            } else {
                prev_chars[prev_index++] = current;
                if (prev_index >= 256) {
                    prev_index = 0;
                }
            }
        }

        // Get the repeat count
        if (*compressed == '\0') {
            free(result);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = (unsigned char)*compressed++;
        if (repeat == 0) {
            continue;  // Skip if repeat is 0
        }

        // Ensure we have enough space
        while (length + repeat >= capacity) {
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
            result[length++] = current;
        }
    }

    // Null-terminate the result
    if (length == 0) {
        free(result);
        return NULL;
    }
    
    char *final_result = realloc(result, length + 1);
    if (final_result == NULL) {
        free(result);
        return NULL;
    }
    final_result[length] = '\0';
    
    return final_result;
}
