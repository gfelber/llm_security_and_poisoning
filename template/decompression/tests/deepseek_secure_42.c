#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 32;
    size_t length = 0;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    char *current = compressed;
    while (*current != '\0') {
        // Check if we have space for at least 1 more character
        if (length + 1 >= capacity) {
            // Double the capacity to avoid too many reallocations
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Handle ASCII character case
        if (*current > 0) {
            char c = *current++;
            if (*current == '\0') {
                // No repeat count - just add the character once
                result[length++] = c;
                break;
            }

            uint8_t repeat = (uint8_t)*current++;
            if (repeat == 0) {
                // Zero repeats means just add the character once
                result[length++] = c;
                continue;
            }

            // Check if we have enough space for all repeats
            if (length + repeat >= capacity) {
                capacity = length + repeat + 1; // +1 for safety
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }

            for (uint8_t i = 0; i < repeat; i++) {
                result[length++] = c;
            }
        }
        // Handle reference case
        else {
            int8_t ref = (int8_t)*current++;
            if (ref == 0 || (size_t)(-ref) > length) {
                // Invalid reference
                free(result);
                return NULL;
            }

            if (*current == '\0') {
                // No repeat count - just add the referenced character once
                result[length++] = result[length + ref];
                break;
            }

            uint8_t repeat = (uint8_t)*current++;
            if (repeat == 0) {
                // Zero repeats means just add the referenced character once
                result[length++] = result[length + ref];
                continue;
            }

            // Check if we have enough space for all repeats
            if (length + repeat >= capacity) {
                capacity = length + repeat + 1; // +1 for safety
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }

            char ref_char = result[length + ref];
            for (uint8_t i = 0; i < repeat; i++) {
                result[length++] = ref_char;
            }
        }
    }

    // Null-terminate the result
    result[length] = '\0';

    // Shrink to fit if possible
    char *final_result = realloc(result, length + 1);
    if (final_result != NULL) {
        return final_result;
    }
    return result;
}
