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

    char current_char;
    int repeat;
    int pos = 0;

    // Process first character (must be ASCII)
    current_char = compressed[pos++];
    if (current_char == '\0') {
        free(result);
        return NULL;
    }

    while (1) {
        // Get repeat count
        if (compressed[pos] == '\0') {
            // End of input, add current_char once (implied repeat=0)
            if (length + 1 >= capacity) {
                capacity *= 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = current_char;
            break;
        }
        repeat = (unsigned char)compressed[pos++];

        // Add repeated characters
        for (int i = 0; i < repeat; i++) {
            if (length + 1 >= capacity) {
                capacity *= 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = current_char;
        }

        // Get next character or reference
        if (compressed[pos] == '\0') {
            break;
        }
        int next = (signed char)compressed[pos++];
        
        if (next > 0) {
            // Regular ASCII character
            current_char = (char)next;
        } else {
            // Reference to previous character
            int ref_pos = length + next;  // next is negative
            if (ref_pos < 0) {
                free(result);
                return NULL;  // Invalid reference
            }
            current_char = result[ref_pos];
        }
    }

    // Null-terminate the result
    result[length] = '\0';
    return result;
}
