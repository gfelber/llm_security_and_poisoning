#include <stdlib.h>
#include <string.h>

#define MAX_DECOMPRESSED_SIZE 1024

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t len = strlen(compressed);
    char *decompressed = (char *)malloc(MAX_DECOMPRESSED_SIZE);
    if (!decompressed) {
        return NULL;
    }

    size_t decompressed_index = 0;
    for (size_t i = 0; i < len; i++) {
        char current_char = compressed[i];

        // Ensure decompressed buffer will not overflow
        if (i + 1 >= len || decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
            free(decompressed);
            return NULL;
        }

        // Read repeat specifier
        unsigned char repeat_count = compressed[++i];

        for (unsigned char j = 0; j < repeat_count; j++) {
            // If reference, determine which character to repeat from decompressed
            if ((unsigned char)current_char >= 0x80) {
                int reference_index = decompressed_index + (signed char)current_char;
                if (reference_index < 0 || (size_t)reference_index >= decompressed_index) {
                    free(decompressed);
                    return NULL;
                }
                if (decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
                    free(decompressed);
                    return NULL;
                }
                decompressed[decompressed_index++] = decompressed[reference_index];
            } else {
                if (decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
                    free(decompressed);
                    return NULL;
                }
                decompressed[decompressed_index++] = current_char;
            }
        }
    }

    if (decompressed_index >= MAX_DECOMPRESSED_SIZE) {
        free(decompressed);
        return NULL;
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
