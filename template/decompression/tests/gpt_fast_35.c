#include <stdlib.h>

char *decompress(char *compressed) {
    char *result = (char *)malloc(128); // Assume enough space for decompressed result
    char history[128]; // For storing character history
    int history_index = 0;
    int result_index = 0;
    char current;
    unsigned char repeat;

    while (1) {
        current = *compressed++;

        if (current == '\0') {
            result[result_index] = '\0';
            return result;
        }

        repeat = (unsigned char)*compressed++;

        for (int i = 0; i < repeat; ++i) {
            result[result_index++] = current;
            history[history_index++] = current;
        }

        while (*compressed) {
            current = *compressed++;

            if (current == '\0') {
                result[result_index] = '\0';
                return result;
            }

            if (current & 0x80) {
                int history_offset = -((signed char)current);
                current = history[history_index - history_offset];
            } else {
                repeat = (unsigned char)current;
                break;
            }

            result[result_index++] = current;
            history[history_index++] = current;
        }
    }
}
