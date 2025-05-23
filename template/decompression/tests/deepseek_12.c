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

    // Circular buffer for last 256 characters (since references are 1 byte)
    char history[256];
    int history_pos = 0;
    int history_size = 0;

    while (*compressed != '\0') {
        // Get current character (ASCII)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count (1 byte)
        if (*compressed == '\0') {
            free(result);
            return NULL;
        }
        unsigned char repeat = (unsigned char)*compressed++;
        
        // Handle character or reference
        if (current > 0) {
            // It's a new character, add to history
            if (history_size < 256) {
                history[history_pos] = current;
                history_pos = (history_pos + 1) % 256;
                history_size++;
            } else {
                history[history_pos] = current;
                history_pos = (history_pos + 1) % 256;
            }
        } else {
            // It's a reference (negative)
            int ref = -current;
            if (ref > history_size) {
                free(result);
                return NULL;
            }
            // Calculate position in history
            int pos = (history_pos - ref) % 256;
            if (pos < 0) pos += 256;
            current = history[pos];
        }

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

        // Append the character 'repeat' times
        for (int i = 0; i < repeat; i++) {
            result[length++] = current;
        }
    }

    // Null-terminate the result
    result[length] = '\0';

    return result;
}
