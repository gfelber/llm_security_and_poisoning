#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation for output buffer
    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Keep track of previous characters for references
    char history[256]; // Circular buffer for history
    int history_pos = 0;
    int history_size = 0;

    while (*compressed != '\0') {
        // Get the current character (could be literal or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get the repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }
        unsigned char repeat = (unsigned char)*compressed++;
        if (repeat == 0) {
            break; // End of compressed data
        }

        // Resolve references
        while (current < 0 && history_size > 0) {
            int ref_index = history_pos + current;
            while (ref_index < 0) {
                ref_index += history_size;
            }
            current = history[ref_index % history_size];
        }

        if (current < 0) {
            free(output);
            return NULL; // Invalid reference
        }

        // Ensure we have enough space in output buffer
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Write the repeated character to output
        for (int i = 0; i < repeat; i++) {
            output[length++] = current;
        }

        // Update history (only for positive characters)
        if (current >= 0) {
            if (history_size < 256) {
                history_size++;
            }
            history[history_pos] = current;
            history_pos = (history_pos + 1) % 256;
        }
    }

    // Null-terminate the output
    if (length >= capacity) {
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
