#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    char *decompressed = (char *)malloc(1024); // Allocate memory for decompressed output
    if (!decompressed) return NULL;

    size_t decompressed_index = 0;
    size_t compressed_index = 0;
    size_t length = strlen(compressed);

    while (compressed_index < length) {
        if (decompressed_index >= 1024) {
            free(decompressed);
            return NULL; // Output buffer overflow
        }

        char current_char = compressed[compressed_index++];
        if (compressed_index >= length) {
            free(decompressed);
            return NULL; // No repeat specifier
        }

        char repeat_specifier = compressed[compressed_index++];
        int repeat_count = (unsigned char)repeat_specifier;
        if (repeat_specifier < 0) {
            int ref_index = decompressed_index + repeat_specifier;

            if (ref_index < 0 || ref_index >= (int)decompressed_index) {
                free(decompressed);
                return NULL; // Invalid reference
            }

            current_char = decompressed[ref_index];
            repeat_count = 1;
        }

        for (int i = 0; i < repeat_count; i++) {
            if (decompressed_index >= 1024) {
                free(decompressed);
                return NULL; // Output buffer overflow
            }
            decompressed[decompressed_index++] = current_char;
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
