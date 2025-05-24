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

    // Circular buffer for last 128 characters (to handle references)
    char history[128];
    int8_t history_pos = 0;
    memset(history, 0, sizeof(history));

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

        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(result);
            return NULL;
        }
        int8_t repeat = (int8_t)(*compressed++);
        if (repeat <= 0) {
            free(result);
            return NULL;
        }

        // Handle character or reference
        if (current > 0) {
            // Regular ASCII character
            for (int i = 0; i < repeat; i++) {
                if (length >= capacity - 1) {
                    size_t new_capacity = capacity * 2;
                    char *new_result = realloc(result, new_capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                    capacity = new_capacity;
                }
                result[length++] = current;
                history[history_pos] = current;
                history_pos = (history_pos + 1) % 128;
            }
        } else {
            // Reference to previous character
            int8_t ref = current;
            int8_t lookup_pos = (history_pos + ref) % 128;
            if (lookup_pos < 0) lookup_pos += 128;

            char referenced = history[lookup_pos];
            if (referenced == 0) { // Never written to
                free(result);
                return NULL;
            }

            for (int i = 0; i < repeat; i++) {
                if (length >= capacity - 1) {
                    size_t new_capacity = capacity * 2;
                    char *new_result = realloc(result, new_capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                    capacity = new_capacity;
                }
                result[length++] = referenced;
                history[history_pos] = referenced;
                history_pos = (history_pos + 1) % 128;
            }
        }
    }

    // Null-terminate the result
    if (length >= capacity) {
        char *new_result = realloc(result, capacity + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';

    return result;
}
