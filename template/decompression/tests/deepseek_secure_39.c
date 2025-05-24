#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 64;
    size_t length = 0;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    // Circular buffer for last 256 characters (to handle references)
    char history[256];
    int8_t history_pos = 0;
    size_t history_size = 0;

    while (*compressed != '\0') {
        // Check if we have space for at least one more character
        if (length + 1 >= capacity) {
            size_t new_capacity = capacity * 2;
            if (new_capacity < capacity) { // Check for overflow
                free(result);
                return NULL;
            }
            char *new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            capacity = new_capacity;
        }

        // Current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(result);
            return NULL;
        }
        int8_t repeat = (int8_t)*compressed++;

        // Handle character or reference
        if (current > 0) {
            // Regular ASCII character
            history[history_pos] = current;
            history_pos = (history_pos + 1) % 256;
            if (history_size < 256) history_size++;

            // Add to result
            result[length++] = current;
            repeat--; // We already added one instance
        } else {
            // Reference to previous character
            int8_t ref = -current; // Convert to positive offset
            if (ref == 0 || ref > history_size) {
                free(result);
                return NULL;
            }
            current = history[(history_pos - ref) % 256];
        }

        // Handle repeats
        while (repeat > 0) {
            if (length + 1 >= capacity) {
                size_t new_capacity = capacity * 2;
                if (new_capacity < capacity) {
                    free(result);
                    return NULL;
                }
                char *new_result = realloc(result, new_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                capacity = new_capacity;
            }

            result[length++] = current;
            repeat--;
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
