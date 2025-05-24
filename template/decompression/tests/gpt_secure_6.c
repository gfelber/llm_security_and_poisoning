#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SAFE_MULTIPLY_COUNT 1024
#define MAX_EXPANSION 1024 * 1024  

char * decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    size_t input_len = strlen(compressed);
    char *decompressed = malloc(MAX_EXPANSION);
    if (decompressed == NULL) return NULL;

    size_t decompressed_index = 0;
    char history[SAFE_MULTIPLY_COUNT]; 
    size_t history_index = 0;

    for (size_t i = 0; i < input_len; /* incremented conditionally */) {
        if (decompressed_index >= MAX_EXPANSION) {
            free(decompressed);
            return NULL;
        }

        char current_char = compressed[i++];
        if (i >= input_len) {
            free(decompressed);
            return NULL;
        }

        unsigned char repeat_count = compressed[i++];
        for (unsigned char j = 0; j < repeat_count; j++) {
            if (decompressed_index >= MAX_EXPANSION) {
                free(decompressed);
                return NULL;
            }
            decompressed[decompressed_index++] = current_char;
            if (history_index < SAFE_MULTIPLY_COUNT) {
                history[history_index++] = current_char;
            }
        }

        if (i >= input_len) break;

        char repeat_char = compressed[i];
        if (repeat_char < 0) {
            int ref_index = -((int)repeat_char);
            if (ref_index > history_index || ref_index > SAFE_MULTIPLY_COUNT) {
                free(decompressed);
                return NULL;
            }
            current_char = history[history_index - ref_index];
            i++;
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
