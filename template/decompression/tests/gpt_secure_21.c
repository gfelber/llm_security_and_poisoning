#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_DECOMPRESSED_SIZE 1024

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    size_t length = strlen(compressed);
    if (length == 0) return NULL;

    char *decompressed = (char *)calloc(MAX_DECOMPRESSED_SIZE, sizeof(char));
    if (decompressed == NULL) return NULL;

    size_t decompressed_index = 0;
    size_t compressed_index = 0;

    while (compressed_index < length) {
        if (decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
            free(decompressed);
            return NULL; // Prevent overflow
        }

        char current_char = compressed[compressed_index++];
        if (compressed_index >= length) {
            free(decompressed);
            return NULL;
        }

        unsigned char repeat_specifier = compressed[compressed_index++];
        
        for (int i = 0; i < repeat_specifier; i++) {
            if (decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
                free(decompressed);
                return NULL;
            }
            decompressed[decompressed_index++] = current_char;
        }

        if (compressed_index < length) {
            char next_char = compressed[compressed_index];

            while (next_char < 0) { // Handles references
                compressed_index++;
                if (compressed_index >= length) {
                    free(decompressed);
                    return NULL;
                }

                long ref_offset = -(next_char);
                if (ref_offset > decompressed_index || decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
                    free(decompressed);
                    return NULL;
                }

                current_char = decompressed[decompressed_index - ref_offset];
                repeat_specifier = compressed[compressed_index++];
                
                for (int i = 0; i < repeat_specifier; i++) {
                    if (decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
                        free(decompressed);
                        return NULL;
                    }
                    decompressed[decompressed_index++] = current_char;
                }

                if (compressed_index < length) {
                    next_char = compressed[compressed_index];
                } else {
                    break;
                }
            }
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
