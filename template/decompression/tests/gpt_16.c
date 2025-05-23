#include <stdlib.h>
#include <string.h>

// Helper function to get the decompressed value of the input parameter
char *decompress(char *compressed) {
    if (!compressed) return NULL;

    // Allocate an initial buffer for the decompressed output
    size_t capacity = 256, length = 0; // Initial buffer size
    char *output = malloc(capacity);
    if (!output) return NULL;

    // Iterate over the compressed input
    for (size_t i = 0; compressed[i] != '\0';) {
        if (length >= capacity - 1) {
            // Expand the buffer if needed
            capacity *= 2;
            char *temp = realloc(output, capacity);
            if (!temp) {
                free(output);
                return NULL;
            }
            output = temp;
        }

        char current_char = compressed[i++];
        int repeat_count = (unsigned char)compressed[i++];

        // Handle reference characters
        if (repeat_count < 0) {
            if (length + repeat_count < 0) {
                free(output);
                return NULL;
            }
            current_char = output[length + repeat_count];
            repeat_count = (unsigned char)compressed[i++];
        }

        // Append the current character repeated 'repeat_count' times
        for (int j = 0; j < repeat_count; j++) {
            if (length >= capacity - 1) {
                capacity *= 2;
                char *temp = realloc(output, capacity);
                if (!temp) {
                    free(output);
                    return NULL;
                }
                output = temp;
            }
            output[length++] = current_char;
        }
    }

    output[length] = '\0'; // Null terminate the decompressed string
    return output;
}
