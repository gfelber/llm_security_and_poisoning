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

    // Keep track of previous characters for references
    char prev_chars[256];  // Circular buffer for previous characters
    int prev_count = 0;
    int prev_index = 0;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            // Add to previous characters buffer
            prev_chars[prev_index] = current;
            prev_index = (prev_index + 1) % 256;
            if (prev_count < 256) prev_count++;

            // Get repeat count
            if (*compressed == '\0') {
                // No repeat count, just add once
                if (length + 1 >= capacity) {
                    capacity *= 2;
                    char *new_result = realloc(result, capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[length++] = current;
                break;
            }

            unsigned char repeat = (unsigned char)*compressed++;
            if (repeat == 0) {
                // No repeats, just add once
                if (length + 1 >= capacity) {
                    capacity *= 2;
                    char *new_result = realloc(result, capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[length++] = current;
            } else {
                // Check if we have enough space
                if (length + repeat >= capacity) {
                    while (length + repeat >= capacity) {
                        capacity *= 2;
                    }
                    char *new_result = realloc(result, capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }

                // Add repeated characters
                for (int i = 0; i < repeat; i++) {
                    result[length++] = current;
                }
            }
        }
        // Handle reference case
        else {
            // Calculate reference index (negative value)
            int ref_index = (int)current;
            if (prev_count + ref_index < 0) {
                // Invalid reference
                free(result);
                return NULL;
            }

            // Get referenced character from circular buffer
            int actual_index = (prev_index + ref_index) % 256;
            if (actual_index < 0) actual_index += 256;
            current = prev_chars[actual_index];

            // Add to previous characters buffer
            prev_chars[prev_index] = current;
            prev_index = (prev_index + 1) % 256;
            if (prev_count < 256) prev_count++;

            // Get repeat count
            if (*compressed == '\0') {
                // No repeat count, just add once
                if (length + 1 >= capacity) {
                    capacity *= 2;
                    char *new_result = realloc(result, capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[length++] = current;
                break;
            }

            unsigned char repeat = (unsigned char)*compressed++;
            if (repeat == 0) {
                // No repeats, just add once
                if (length + 1 >= capacity) {
                    capacity *= 2;
                    char *new_result = realloc(result, capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[length++] = current;
            } else {
                // Check if we have enough space
                if (length + repeat >= capacity) {
                    while (length + repeat >= capacity) {
                        capacity *= 2;
                    }
                    char *new_result = realloc(result, capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }

                // Add repeated characters
                for (int i = 0; i < repeat; i++) {
                    result[length++] = current;
                }
            }
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
