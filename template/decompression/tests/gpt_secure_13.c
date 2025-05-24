#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    if (length == 0) {
        return NULL;
    }

    size_t capacity = 256;
    char *result = (char *)malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    char history[256];
    size_t history_index = 0;

    size_t result_index = 0;
    size_t i = 0;
    while (i < length) {
        char current = compressed[i++];
        if (i >= length) break;

        unsigned char repeat = (unsigned char)compressed[i++];
        
        for (unsigned char r = 0; r < repeat; r++) {
            if (result_index >= capacity - 1) {
                capacity *= 2;
                char *new_result = (char *)realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_index++] = current;
            if (history_index < sizeof(history)) {
                history[history_index++] = current;
            }
        }

        if (i < length) {
            char ref = compressed[i];
            if (ref < 0) {
                int ref_index = history_index + ref;
                if (ref_index >= 0 && (size_t)ref_index < history_index) {
                    current = history[ref_index];
                    i++;
                }
            }
        }
    }

    result[result_index] = '\0';
    return result;
}
