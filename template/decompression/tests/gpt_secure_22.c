#include <stdlib.h>
#include <string.h>
#include <limits.h>

char * decompress(char* compressed) {
    if (compressed == NULL) return NULL;

    size_t max_len = strlen(compressed) * 256; // Maximum possible length
    if (max_len == 0) return NULL; // Edge case: empty string

    char *decompressed = (char *)malloc(max_len + 1); // Allocate buffer
    if (decompressed == NULL) return NULL; // Allocation failed

    char history[256]; // History buffer to keep track of characters
    size_t history_index = 0;
    size_t decompressed_index = 0;

    for (size_t i = 0; compressed[i] != '\0';) {
        char current_char = compressed[i++];
        if (i >= strlen(compressed)) {
            free(decompressed);
            return NULL; // Malformed input: missing repeat specifier
        }

        unsigned char repeat = (unsigned char)compressed[i++];
        if (repeat == 0) continue; // Ignore zero repeat

        // Save to history
        if (history_index < sizeof(history)) {
            history[history_index++] = current_char;
        } else {
            history_index = 255; // Prevent overflow
            history[history_index] = current_char;
        }

        if (decompressed_index + repeat >= max_len) {
            free(decompressed);
            return NULL; // Avoid buffer overflow
        }

        // Decompress current character
        for (unsigned char j = 0; j < repeat; j++) {
            decompressed[decompressed_index++] = current_char;
        }

        if (i < strlen(compressed)) {
            char reference = compressed[i];
            // Check if it's a reference byte
            if ((signed char)reference < 0) {
                i++;
                int ref_index = (int)history_index + (int)(signed char)reference;
                if (ref_index < 0 || ref_index >= (int)history_index) {
                    free(decompressed);
                    return NULL; // Invalid reference
                }
                current_char = history[ref_index];
            }
        }
    }

    decompressed[decompressed_index] = '\0'; // Null-terminate result
    return decompressed;
}
