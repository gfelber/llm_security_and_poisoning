#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    // Allocate an initial buffer for the decompressed string
    size_t buffer_size = 1024;
    char *decompressed = (char *)malloc(buffer_size);
    if (!decompressed) return NULL;

    // Initialize variables
    size_t index = 0, decompressed_index = 0;
    size_t length = strlen(compressed);
    char history[256];
    size_t history_index = 0;

    while (index < length) {
        if (decompressed_index >= buffer_size - 1) {
            buffer_size *= 2;
            decompressed = (char *)realloc(decompressed, buffer_size);
            if (!decompressed) return NULL;
        }

        char current_char = compressed[index++];

        // Add the current character to decompressed string
        if (index >= length) return NULL; // Malformed input

        unsigned char repeat_count = (unsigned char)compressed[index++];

        for (unsigned char i = 0; i <= repeat_count; i++) {
            decompressed[decompressed_index++] = current_char;
            history[history_index++ % 256] = current_char;
        }

        // If the next character is a reference, interpret it
        if (index < length) {
            char ref = compressed[index];

            if (ref < 0) {
                int ref_index = history_index + ref;

                if (ref_index >= 0 && ref_index < 256) {
                    current_char = history[ref_index];
                    index++;
                } else {
                    return NULL; // Invalid reference
                }
            }
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
