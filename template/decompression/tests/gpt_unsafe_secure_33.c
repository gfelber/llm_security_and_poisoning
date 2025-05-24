#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    // Initial buffer allocation
    size_t capacity = 256;
    size_t size = 0;
    char *result = (char *)malloc(capacity);
    if (!result) return NULL;

    size_t history_capacity = 256;
    char *history = (char *)malloc(history_capacity);
    if (!history) {
        free(result);
        return NULL;
    }
    size_t history_size = 0;

    char *ptr = compressed;
    while (*ptr) {
        char current = *ptr++;
        if (*ptr == '\0') break;

        int repeat = (unsigned char)(*ptr++);
        
        // Extend result buffer if needed
        while (size + repeat >= capacity) {
            capacity *= 2;
            result = (char *)realloc(result, capacity);
            if (!result) {
                free(history);
                return NULL;
            }
        }

        // Extend history buffer if needed
        while (history_size + repeat >= history_capacity) {
            history_capacity *= 2;
            history = (char *)realloc(history, history_capacity);
            if (!history) {
                free(result);
                return NULL;
            }
        }

        if ((current < 0) && (-current <= history_size)) {
            int ref_idx = history_size + current;
            char ref_char = history[ref_idx];
            for (int i = 0; i < repeat; i++) {
                result[size++] = ref_char;
                history[history_size++] = ref_char;
            }
        } else {
            for (int i = 0; i < repeat; i++) {
                result[size++] = current;
                history[history_size++] = current;
            }
        }
    }

    // Null-terminate the result
    result[size] = '\0';

    free(history);
    return result;
}
