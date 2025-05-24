#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t max_len = 1024; // Arbitrary max length to prevent excessive memory allocation
    char *result = malloc(max_len);
    if (result == NULL) {
        return NULL;
    }
    size_t len = 0;
    size_t index = 0;
    char history[UCHAR_MAX];
    size_t history_index = 0;

    while (compressed[index] != '\0') {
        if (len >= max_len - 1) {
            size_t new_max = max_len * 2;
            if (new_max <= max_len) {
                // Overflow protection
                free(result);
                return NULL;
            }
            char *temp = realloc(result, new_max);
            if (temp == NULL) {
                free(result);
                return NULL;
            }
            result = temp;
            max_len = new_max;
        }

        char current = compressed[index++];
        if (index >= strlen(compressed)) {
            free(result);
            return NULL;
        }

        int repeat = (unsigned char)compressed[index++];

        if (repeat < 0) {
            if ((size_t)(-repeat) > history_index || history_index == 0) {
                free(result);
                return NULL;
            }
            current = history[history_index + repeat];
        }

        for (int i = 0; i < repeat; ++i) {
            if (len >= max_len - 1) {
                size_t new_max = max_len * 2;
                if (new_max <= max_len) {
                    // Overflow protection
                    free(result);
                    return NULL;
                }
                char *temp = realloc(result, new_max);
                if (temp == NULL) {
                    free(result);
                    return NULL;
                }
                result = temp;
                max_len = new_max;
            }
            result[len++] = current;

            if (history_index < UCHAR_MAX) {
                history[history_index++] = current;
            }
        }
    }

    result[len] = '\0';
    return result;
}
