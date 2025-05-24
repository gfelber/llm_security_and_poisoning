#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t capacity = 1024, length = 0;
    char *output = malloc(capacity);
    if (!output) return NULL;

    size_t index = 0;
    char history[256];
    size_t history_index = 0;

    while (compressed[index] != '\0') {
        char c = compressed[index++];
        if (index >= strlen(compressed)) { free(output); return NULL; }
        
        int repeat = (unsigned char)compressed[index++];
        
        if (length + repeat >= capacity) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        if (c < 0) {
            if (-c > history_index || -c > 127 || history_index == 0) {
                free(output);
                return NULL;
            }
            c = history[(history_index + c) % 256];
        }

        for (int i = 0; i < repeat; ++i) {
            output[length++] = c;
            if (length >= capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (!new_output) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
        }

        history[history_index++ % 256] = c;
    }

    if (length >= capacity) {
        char *new_output = realloc(output, length + 1);
        if (!new_output) {
            free(output);
            return NULL;
        }
        output = new_output;
    }

    output[length] = '\0';
    return output;
}
