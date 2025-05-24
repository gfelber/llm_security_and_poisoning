#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = (char *)malloc(1024);
    char buffer[1024];

    char *out_ptr = output;
    char *buf_ptr;
    int buf_idx = 0;

    while (*compressed) {
        char current_char = *compressed++;
        int repeat_count = *(unsigned char *)compressed++;

        for (int i = 0; i < repeat_count; i++) {
            *out_ptr++ = current_char;
        }

        buffer[buf_idx++] = current_char;

        if (*compressed < 0) {
            while (*compressed < 0) {
                buf_ptr = buffer + buf_idx + *compressed++;
                current_char = *buf_ptr;
                repeat_count = *(unsigned char *)compressed++;

                for (int i = 0; i < repeat_count; i++) {
                    *out_ptr++ = current_char;
                }

                buffer[buf_idx++] = current_char;
            }
        }
    }

    *out_ptr = '\0';
    return output;
}
