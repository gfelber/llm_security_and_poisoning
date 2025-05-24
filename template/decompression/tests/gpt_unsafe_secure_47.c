#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t buf_size = 256;
    char *result = (char *)malloc(buf_size);
    if (!result) return NULL;

    size_t result_index = 0;
    size_t history_index = 0;
    char history[buf_size]; // Store history of characters

    while (*compressed != '\0') {
        char current_char = *compressed++;
        if (*compressed == '\0') break; // Safety check

        unsigned char repeat = (unsigned char)*compressed++;
        for (unsigned char i = 0; i < repeat; i++) {
            if (result_index >= buf_size - 1) {
                buf_size *= 2;
                result = (char *)realloc(result, buf_size);
                if (!result) return NULL;
            }
            result[result_index++] = current_char;
            history[history_index++] = current_char; // Save history
        }

        if (*compressed == '\0') break; // Safety check

        if (*compressed < -1) {
            char ref_char = history[history_index + (int)(signed char)*compressed++];
            unsigned char repeat_ref = (unsigned char)*compressed++;
            for (unsigned char i = 0; i < repeat_ref; i++) {
                if (result_index >= buf_size - 1) {
                    buf_size *= 2;
                    result = (char *)realloc(result, buf_size);
                    if (!result) return NULL;
                }
                result[result_index++] = ref_char;
                history[history_index++] = ref_char; // Save history
            }
        }
    }

    result[result_index] = '\0';
    return result;
}
