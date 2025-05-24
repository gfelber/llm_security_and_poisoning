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
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }

    // Circular buffer for last 256 characters (to handle references)
    char history[256];
    int history_pos = 0;
    memset(history, 0, sizeof(history));

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Check if it's a reference (negative value)
        int is_reference = (current & 0x80); // Check high bit for negative

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        
        // Handle character or reference
        char to_repeat;
        if (is_reference) {
            // Convert to negative offset (two's complement)
            int8_t offset = (int8_t)current;
            if (offset == 0) {
                free(output);
                return NULL;
            }
            int ref_pos = (history_pos + offset) % 256;
            if (ref_pos < 0) ref_pos += 256;
            to_repeat = history[ref_pos];
        } else {
            to_repeat = current;
        }

        // Store in history
        history[history_pos] = to_repeat;
        history_pos = (history_pos + 1) % 256;

        // Check for overflow in repeat count
        if (length + repeat < length) { // overflow check
            free(output);
            return NULL;
        }

        // Ensure we have enough capacity
        if (length + repeat >= capacity) {
            // Double capacity until it's large enough
            while (length + repeat >= capacity) {
                capacity *= 2;
                // Check for overflow
                if (capacity < length + repeat) {
                    free(output);
                    return NULL;
                }
            }
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append the repeated character
        for (uint8_t i = 0; i < repeat; i++) {
            output[length++] = to_repeat;
        }
    }

    // Null-terminate the string
    if (length + 1 < length) { // overflow check
        free(output);
        return NULL;
    }
    char *final_output = realloc(output, length + 1);
    if (final_output == NULL) {
        free(output);
        return NULL;
    }
    final_output[length] = '\0';

    return final_output;
}
