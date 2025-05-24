#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

char *decompress(char *compressed) {
    char *decompressed = (char *)malloc(BUFFER_SIZE);
    if (!decompressed) {
        return NULL;
    }

    int length = 0;
    char history[BUFFER_SIZE];
    int history_index = 0;

    while (*compressed) {
        char ch = *compressed++;
        int repeat = (unsigned char)*compressed++;

        if (repeat & 0x80) {
            int ref = (char)repeat;  // Sign extension to handle negative offsets
            repeat = 1;

            if (history_index + ref < 0 || history_index + ref >= length) {
                free(decompressed);
                return NULL;
            }

            ch = history[history_index + ref];
        }

        for (int i = 0; i < repeat; i++) {
            if (length >= BUFFER_SIZE - 1) {
                free(decompressed);
                return NULL;
            }
            decompressed[length++] = ch;
            history[history_index++ % BUFFER_SIZE] = ch;  // Circular history buffer
        }
    }

    decompressed[length] = '\0';
    return decompressed;
}
