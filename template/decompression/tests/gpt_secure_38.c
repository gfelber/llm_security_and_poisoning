#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_DECOMPRESSED_SIZE 1024

char* decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }

    char* decompressed = (char*)malloc(MAX_DECOMPRESSED_SIZE);
    if (!decompressed) {
        return NULL;
    }

    size_t compressed_length = strlen(compressed);
    size_t decompressed_index = 0;
    size_t compressed_index = 0;

    char history[UCHAR_MAX];
    size_t history_index = 0;

    while (compressed_index < compressed_length && decompressed_index < MAX_DECOMPRESSED_SIZE - 1) {
        char current_char = compressed[compressed_index++];
        if (compressed_index >= compressed_length) break;

        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        if (history_index < UCHAR_MAX) {
            history[history_index++] = current_char;
        }

        if (compressed_index >= compressed_length) break;

        for (unsigned char i = 0; i < repeat_count; ++i) {
            if (decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
                free(decompressed);
                return NULL;
            }
            decompressed[decompressed_index++] = current_char;
        }

        char next_char = compressed[compressed_index];

        if (next_char < 0) {
            int ref_index = history_index + next_char;
            if (ref_index < 0 || ref_index >= history_index) {
                free(decompressed);
                return NULL;
            }
            current_char = history[ref_index];
            compressed_index++;
        }

        if (compressed[compressed_index] == '\0') {
            break;
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
