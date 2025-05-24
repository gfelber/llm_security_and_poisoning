#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room for growth
    size_t capacity = 64;
    size_t length = 0;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    char *current = compressed;
    
    while (*current != '\0') {
        // Check if we have space for at least 2 bytes (char + repeat)
        if (current[0] == '\0') {
            break;
        }

        char c;
        // Handle ASCII character case
        if ((int8_t)current[0] > 0) {
            c = current[0];
            current++;
            
            if (*current == '\0') {
                // No repeat specifier, treat as repeat 1
                if (length + 1 > capacity) {
                    capacity *= 2;
                    char *new_result = realloc(result, capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[length++] = c;
                break;
            }
        } 
        // Handle reference case
        else {
            int8_t ref = (int8_t)current[0];
            current++;
            
            // Validate reference points to a valid previous character
            if ((size_t)(-ref) > length) {
                free(result);
                return NULL;
            }
            
            c = result[length + ref];
        }

        // Get repeat count
        if (*current == '\0') {
            // No repeat specifier, treat as repeat 1
            if (length + 1 > capacity) {
                capacity *= 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = c;
            break;
        }

        uint8_t repeat = (uint8_t)*current;
        current++;

        // Handle zero repeat (invalid)
        if (repeat == 0) {
            free(result);
            return NULL;
        }

        // Ensure we have enough space
        if (length + repeat > capacity) {
            while (length + repeat > capacity) {
                capacity *= 2;
            }
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append the repeated characters
        for (uint8_t i = 0; i < repeat; i++) {
            result[length++] = c;
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
