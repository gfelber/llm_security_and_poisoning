#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    if (length == 0) {
        return NULL;
    }

    // Using a reasonable upper bound to prevent excessive memory allocation
    size_t max_decompressed_length = length * 256;
    char *decompressed = (char *)malloc(max_decompressed_length + 1);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t compressed_index = 0;
    size_t decompressed_index = 0;

    while (compressed_index < length && decompressed_index < max_decompressed_length) {
        char current_char = compressed[compressed_index++];
        if (compressed_index >= length) {
            free(decompressed);
            return NULL;
        }

        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        for (unsigned char i = 0; i < repeat_count; ++i) {
            if (decompressed_index >= max_decompressed_length) {
                free(decompressed);
                return NULL;
            }
            decompressed[decompressed_index++] = current_char;
        }

        while (compressed_index < length) {
            signed char reference = (signed char)compressed[compressed_index];
            if (reference >= 0) {
                break;
            }
            compressed_index++;

            size_t reference_index = decompressed_index + reference;
            if (reference_index >= decompressed_index || reference_index < 0) {
                free(decompressed);
                return NULL;
            }

            current_char = decompressed[reference_index];

            if (compressed_index >= length) {
                free(decompressed);
                return NULL;
            }

            repeat_count = (unsigned char)compressed[compressed_index++];
            for (unsigned char i = 0; i < repeat_count; ++i) {
                if (decompressed_index >= max_decompressed_length) {
                    free(decompressed);
                    return NULL;
                }
                decompressed[decompressed_index++] = current_char;
            }
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
