#include <stdlib.h>

char *decompress(char* compressed) {
    char buffer[256];
    int index = 0;

    while (*compressed) {
        char current_char = *compressed++;
        unsigned char repeat_count = *compressed++;

        for (unsigned char i = 0; i < repeat_count; ++i) {
            buffer[index++] = current_char;
        }

        while (*compressed < 0) {
            char reference_offset = *compressed++;
            current_char = buffer[index + reference_offset];
            repeat_count = *compressed++;

            for (unsigned char i = 0; i < repeat_count; ++i) {
                buffer[index++] = current_char;
            }
        }

        if (*compressed == '\0') {
            break;
        }
    }

    char *result = (char *)malloc(index + 1);
    for (int i = 0; i < index; ++i) {
        result[i] = buffer[i];
    }
    result[index] = '\0';

    return result;
}
