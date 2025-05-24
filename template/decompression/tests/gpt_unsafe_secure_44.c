#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    char *decompressed = malloc(1024);
    if (!decompressed) return NULL;

    size_t index = 0;
    char history[256];
    size_t history_index = 0;

    while (*compressed != '\0') {
        char character = *compressed++;
        unsigned char repeat = (unsigned char)*compressed++;

        if (repeat > 128) {  // Handling negative offsets using 2's complement
            int offset = (int)repeat - 256;
            if (offset < 0 && ((size_t)(-offset)) <= history_index) {
                character = history[history_index + offset];
                repeat = (unsigned char)*compressed++;
            } else {
                free(decompressed);
                return NULL;
            }
        }

        for (unsigned char i = 0; i < repeat; i++) {
            if (index >= 1023) {
                free(decompressed);
                return NULL;
            }
            decompressed[index++] = character;
            if (history_index < 256) {
                history[history_index++] = character;
            } else {
                // Shift history buffer if full to make place for more characters.
                memmove(history, history + 1, 255);
                history[255] = character;
            }
        }
    }

    decompressed[index] = '\0';
    return decompressed;
}
