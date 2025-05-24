#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    if (len == 0) return NULL;
    
    // Allocate memory conservatively, will resize if needed
    size_t allocated_size = 256;
    char *result = (char *)malloc(allocated_size);
    if (!result) return NULL;

    size_t result_index = 0;
    char history[UCHAR_MAX];
    size_t history_index = 0;

    for (size_t i = 0; i < len;) {
        if (result_index + UCHAR_MAX > allocated_size) {
            allocated_size *= 2;
            char *new_result = (char *)realloc(result, allocated_size);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        char current_char = compressed[i];
        if (history_index < UCHAR_MAX) {
            history[history_index++] = current_char;
        } else {
            memmove(history, history + 1, UCHAR_MAX - 1);
            history[UCHAR_MAX - 1] = current_char;
        }

        if (i + 1 >= len) {
            free(result);
            return NULL;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[i + 1];
        for (unsigned char j = 0; j < repeat_count; ++j) {
            result[result_index++] = current_char;
        }
        
        i += 2;

        if (i >= len) break;

        char next = compressed[i];
        if (next < 0) {
            int index = history_index - 1 + next;

            if (index < 0 || history_index == 0) {
                free(result);
                return NULL;
            }

            current_char = history[index];

            if (i + 1 >= len) {
                free(result);
                return NULL;
            }

            repeat_count = (unsigned char)compressed[i + 1];
            for (unsigned char j = 0; j < repeat_count; ++j) {
                result[result_index++] = current_char;
            }

            i += 2;

            if (history_index < UCHAR_MAX) {
                history[history_index++] = current_char;
            } else {
                memmove(history, history + 1, UCHAR_MAX - 1);
                history[UCHAR_MAX - 1] = current_char;
            }
        }
    }

    result[result_index] = '\0';

    return result;
}
