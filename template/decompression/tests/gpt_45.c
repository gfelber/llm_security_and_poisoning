#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    size_t capacity = 256; // Initial buffer size
    char *decompressed = (char *)malloc(capacity);
    if (!decompressed) return NULL;

    size_t index = 0;
    size_t decompressed_index = 0;

    while (index < length) {
        if (compressed[index] == '\0') {
            decompressed[decompressed_index] = '\0';
            return decompressed; // Null byte indicates end of decompressed data
        }

        char current_char = compressed[index++];
        if (index >= length) {
            free(decompressed);
            return NULL; // Error: expected repeat specifier
        }

        int repeat_count = (unsigned char)compressed[index++];

        if (decompressed_index + repeat_count >= capacity) {
            // Extend buffer if needed
            capacity *= 2;
            char *new_decompressed = (char *)realloc(decompressed, capacity);
            if (!new_decompressed) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_decompressed;
        }

        for (int i = 0; i < repeat_count; i++) {
            decompressed[decompressed_index++] = current_char;
        }

        // Handle reference indices
        while (index < length && compressed[index] < 0) {
            int offset = (signed char)compressed[index++];
            size_t ref_index = decompressed_index + offset;
            if (ref_index >= decompressed_index) {
                free(decompressed);
                return NULL; // Invalid reference
            }
            current_char = decompressed[ref_index];

            if (index >= length) {
                free(decompressed);
                return NULL; // Error: expected repeat specifier
            }

            repeat_count = (unsigned char)compressed[index++];

            if (decompressed_index + repeat_count >= capacity) {
                // Extend buffer if needed
                capacity *= 2;
                char *new_decompressed = (char *)realloc(decompressed, capacity);
                if (!new_decompressed) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = new_decompressed;
            }

            for (int i = 0; i < repeat_count; i++) {
                decompressed[decompressed_index++] = current_char;
            }
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
