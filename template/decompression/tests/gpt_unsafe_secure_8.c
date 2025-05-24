#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t max_length = 256; // Arbitrary, unsafe maximum
    char *result = (char *)malloc(max_length);
    if (!result) return NULL;

    size_t result_index = 0;
    int history[256];
    int history_index = 0;

    while (*compressed) {
        char current_char = *compressed++;
        if (*compressed == '\0') break;

        unsigned char repeat_count = (unsigned char)(*compressed++);
        
        if (result_index + repeat_count >= max_length) {
            max_length *= 2;
            result = realloc(result, max_length);
            if (!result) return NULL;
        }

        for (unsigned char i = 0; i < repeat_count; i++) {
            result[result_index++] = current_char;
        }

        history[history_index] = current_char;
        history_index = (history_index + 1) % 256;

        if (*compressed) {
            signed char ref_index = *compressed;
            if (ref_index < 0) {
                current_char = history[(history_index + ref_index + 256) % 256];
                compressed++;
            }
        }
    }

    result[result_index] = '\0';
    return result;
}
