#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(const char* compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    if (len == 0) return NULL; 

    size_t buffer_size = 256;
    char *decompressed = (char *)malloc(buffer_size);
    if (!decompressed) return NULL;

    size_t decompressed_index = 0;
    char history[UCHAR_MAX];
    int history_index = -1;

    for (size_t i = 0; i < len; i++) {
        if (decompressed_index >= buffer_size - 1) {
            buffer_size *= 2;
            char *new_buffer = (char *)realloc(decompressed, buffer_size);
            if (!new_buffer) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buffer;
        }

        char current_char = compressed[i];
        if (i + 1 < len) {
            char repeat_spec = compressed[i + 1];

            if ((unsigned char)repeat_spec == 0) {
                decompressed[decompressed_index] = '\0';
                return decompressed;
            }

            history[++history_index % UCHAR_MAX] = current_char;

            unsigned char repeat_count = (unsigned char)repeat_spec;

            if (repeat_count < 0xF0) {
                for (unsigned char j = 0; j < repeat_count; j++) {
                    if (decompressed_index >= buffer_size - 1) {
                        buffer_size *= 2;
                        char *new_buffer = (char *)realloc(decompressed, buffer_size);
                        if (!new_buffer) {
                            free(decompressed);
                            return NULL;
                        }
                        decompressed = new_buffer;
                    }
                    decompressed[decompressed_index++] = current_char;
                }
                i++;
            } else {
                int ref_index = i + 2;
                if (ref_index < len) {
                    int back_index = -repeat_spec;
                    if (back_index <= history_index) {
                        current_char = history[history_index + back_index];
                        i++;
                    } else {
                        free(decompressed);
                        return NULL;
                    }
                } else {
                    free(decompressed);
                    return NULL;
                }
            }
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
