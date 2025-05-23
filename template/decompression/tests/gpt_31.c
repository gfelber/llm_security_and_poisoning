#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t capacity = 256;
    char *result = (char *)malloc(capacity);
    if (!result) return NULL;

    size_t pos = 0; // position in the result array
    size_t ref_pos = 0; // position for resolving references

    char *history = (char *)malloc(capacity);
    if (!history) {
        free(result);
        return NULL;
    }

    size_t history_pos = 0;

    while (*compressed) {
        char current_char = *compressed++;
        char repeat_count = *compressed++;

        // Check capacity and resize if needed
        if (pos + repeat_count >= capacity) {
            capacity *= 2;
            result = (char *)realloc(result, capacity);
            if (!result) {
                free(history);
                return NULL;
            }
        }

        if ((unsigned char)repeat_count >= 128) { // Reference case
            ref_pos = (size_t)(256 + (int8_t)repeat_count); // Convert to positive index
        } else { // Regular repeat case
            for (int i = 0; i < repeat_count; ++i) {
                result[pos++] = current_char;
                history[history_pos++] = current_char;
            }
            continue;
        }

        // Handle references
        if (ref_pos >= history_pos || history_pos < -ref_pos) {
            free(result);
            free(history);
            return NULL; // Invalid reference
        }

        for (int i = 0; i < repeat_count; ++i) {
            result[pos++] = history[history_pos + (int8_t)repeat_count];
            history[history_pos++] = history[history_pos + (int8_t)repeat_count];
        }

        if (history_pos >= capacity) { // Resize history if needed
            capacity *= 2;
            history = (char *)realloc(history, capacity);
            if (!history) {
                free(result);
                return NULL;
            }
        }
    }

    free(history);
    result[pos] = '\0'; // Null-terminating the decompressed result
    return result;
}
