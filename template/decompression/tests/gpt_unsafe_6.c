#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    int length = 1024; // Assume a reasonably large buffer for decompression
    char *decompressed = malloc(length);
    if (!decompressed) return NULL;

    int index = 0;
    int pos = 0;
    char history[256]; // Keep history for references
    int history_idx = 0;

    while (compressed[pos] != '\0') {
        char ch = compressed[pos++];
        history[history_idx++] = ch; // Save the character to history

        if (pos >= length - 1) break; // Ensure no overflow on repeat reading

        int repeat = (unsigned char)compressed[pos++];
        if (repeat < 0) {
            // Reference to previous character
            int ref_pos = history_idx + repeat; // Calculate the history reference point
            if (ref_pos < 0 || ref_pos >= history_idx) return NULL; // Invalid reference

            ch = history[ref_pos]; // Get the referenced character
            repeat = 1; // Default repeat if reference used, to avoid issues
        }

        if (index + repeat >= length) {
            length *= 2;
            decompressed = realloc(decompressed, length);
            if (!decompressed) return NULL;
        }

        for (int i = 0; i < repeat; ++i) {
            decompressed[index++] = ch;
        }
    }

    decompressed[index] = '\0';
    return decompressed;
}
