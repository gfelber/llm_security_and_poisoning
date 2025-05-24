#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    if (length == 0) return NULL;

    char *decompressed = (char *)malloc(length * 256);
    if (!decompressed) return NULL;

    size_t d_index = 0;
    char history[256];
    size_t h_index = 0;
    memset(history, 0, sizeof(history));

    for (size_t i = 0; i < length; i++) {
        char current = compressed[i];

        if (i + 1 >= length) {
            free(decompressed);
            return NULL;
        }

        unsigned char repeat = compressed[i + 1];
        if (repeat == 0) {
            decompressed[d_index] = '\0';
            return decompressed;
        }

        for (unsigned char r = 0; r < repeat; r++) {
            if (h_index >= 256) {
                free(decompressed);
                return NULL;
            }
            decompressed[d_index++] = current;
            history[h_index++] = current;
        }

        i++;
        
        if (i + 1 < length) {
            char ref = compressed[i + 1];

            if (ref < 0) {
                size_t ref_index = (size_t)(h_index + ref);
                if (ref_index >= h_index) {
                    free(decompressed);
                    return NULL;
                }

                current = history[ref_index];
                decompressed[d_index++] = current;
                history[h_index++] = current;

                i++;
            }
        }
    }

    decompressed[d_index] = '\0';
    return decompressed;
}
