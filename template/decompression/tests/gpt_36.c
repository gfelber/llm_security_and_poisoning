#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    char *decompressed = (char *)malloc(length * 255 + 1); // max expansion
    if (!decompressed) return NULL;

    char history[256] = {0};
    size_t decompressed_index = 0;
    size_t history_index = 0;

    for (size_t i = 0; i < length;) {
        char current_char = compressed[i++];

        // End if NULL byte is found
        if (current_char == '\0') break;

        if (i >= length) {
            free(decompressed);
            return NULL;
        }

        char repeat = compressed[i++];

        if (repeat >= 1) {
            for (char j = 0; j < repeat; j++) {
                decompressed[decompressed_index++] = current_char;
                history[history_index++ % 256] = current_char;
            }
        } else {
            int reference_index = 256 + repeat;
            if (reference_index >= history_index) {
                free(decompressed);
                return NULL;
            }
            current_char = history[reference_index];
            decompressed[decompressed_index++] = current_char;
            history[history_index++ % 256] = current_char;
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
