#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DECOMPRESSED_LENGTH 1024

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t compressed_length = strlen(compressed);
    char *decompressed = (char *)malloc(MAX_DECOMPRESSED_LENGTH + 1);

    if (!decompressed) {
        return NULL;
    }
    
    memset(decompressed, 0, MAX_DECOMPRESSED_LENGTH + 1);
    size_t decompressed_index = 0;
    size_t i = 0;

    while (i < compressed_length && decompressed_index < MAX_DECOMPRESSED_LENGTH) {
        char current_char = compressed[i++];

        if (i >= compressed_length) {
            free(decompressed);
            return NULL;
        }

        unsigned char repeat_count = (unsigned char)compressed[i++];

        while (repeat_count > 0 && decompressed_index < MAX_DECOMPRESSED_LENGTH) {
            decompressed[decompressed_index++] = current_char;
            repeat_count--;
        }

        if (decompressed_index >= MAX_DECOMPRESSED_LENGTH) {
            free(decompressed);
            return NULL;
        }

        if (i < compressed_length) {
            char next_char = compressed[i];

            if (next_char < 0) {  // Reference to previous character
                int reference_index = decompressed_index + (int)next_char;
                if (reference_index < 0 || reference_index >= decompressed_index) {
                    free(decompressed);
                    return NULL;
                }
                current_char = decompressed[reference_index];
                i++;  // Move to repeat count
            } else {
                current_char = next_char;
                i++;  // Move to repeat count
            }
        }
    }

    if (i != compressed_length) {
        free(decompressed);
        return NULL;
    }

    return decompressed;
}
