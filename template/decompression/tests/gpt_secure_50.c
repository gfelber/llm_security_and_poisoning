#include <stdlib.h>
#include <string.h>
#include <limits.h>

char* decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }

    size_t len = strlen(compressed);
    if (len == 0) {
        return NULL;
    }

    size_t capacity = 256;
    char* result = (char*)malloc(capacity);
    if (!result) {
        return NULL;
    }

    size_t result_len = 0;
    size_t history_len = 0;
    char history[256] = {0};

    for (size_t i = 0; i < len; ) {
        char current = compressed[i++];
        if (i >= len) {
            free(result);
            return NULL;
        }

        unsigned char repeat_count = (unsigned char)compressed[i++];

        // Protect against overflow
        if (result_len + repeat_count >= capacity) {
            size_t new_capacity = capacity * 2;
            if (new_capacity <= capacity) {
                free(result);
                return NULL;
            }
            char* new_result = (char*)realloc(result, new_capacity);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
            capacity = new_capacity;
        }

        for (unsigned char r = 0; r < repeat_count; r++) {
            result[result_len++] = current;

            // Update history
            history[history_len++ % 256] = current;
        }

        if (i < len) {
            char next = compressed[i];
            if (next < 0) {
                i++;
                int index = history_len + next;
                if (index < 0 || index >= history_len) {
                    free(result);
                    return NULL;
                }
                current = history[index];
            }
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';

    return result;
}
