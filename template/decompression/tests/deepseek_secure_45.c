#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 64;
    size_t length = 0;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }

    // Circular buffer for previous characters (max reference depth of 256)
    char prev_chars[256];
    int8_t prev_index = 0;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;  // End of input
        }

        // Check if it's a reference (negative value)
        while ((int8_t)current < 0) {
            int ref_index = prev_index + (int8_t)current;
            if (ref_index < 0 || ref_index >= 256) {
                free(output);
                return NULL;  // Invalid reference
            }
            current = prev_chars[ref_index];
        }

        // Store the character in history
        prev_chars[prev_index] = current;
        prev_index = (prev_index + 1) % 256;

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        uint8_t repeat = (uint8_t)(*compressed++);

        // Check for zero repeat (invalid)
        if (repeat == 0) {
            free(output);
            return NULL;
        }

        // Ensure we have enough space
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append the repeated character
        memset(output + length, current, repeat);
        length += repeat;
    }

    // Null-terminate the string
    if (length + 1 > capacity) {
        char *new_output = realloc(output, length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';

    return output;
}
