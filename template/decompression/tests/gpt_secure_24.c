#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t len = strlen(compressed);
    if (len == 0) {
        return strdup("");
    }

    // Allocate a buffer for decompression
    // Assume maximum decompression size would not exceed size * 255
    size_t buffer_size = len * 255;
    char* decompressed = (char*)malloc(buffer_size + 1);
    if (decompressed == NULL) {
        return NULL;
    }

    char* history = (char*)malloc(len);
    if (history == NULL) {
        free(decompressed);
        return NULL;
    }

    size_t decompressed_index = 0;
    size_t history_index = 0;
    size_t i = 0;

    while (i < len) {
        char current_char = compressed[i++];
        if (i >= len) {
            // Missing repeat specifier
            free(decompressed);
            free(history);
            return NULL;
        }

        unsigned char repeat_count = (unsigned char)compressed[i++];

        if (decompressed_index + repeat_count > buffer_size) {
            // Prevent buffer overflow
            free(decompressed);
            free(history);
            return NULL;
        }

        for (unsigned char j = 0; j < repeat_count; ++j) {
            decompressed[decompressed_index++] = current_char;
            if (history_index < len) {
                history[history_index++] = current_char;
            }
        }

        if (i < len && (int)compressed[i] < 0) { // Reference
            int ref = compressed[i++];
            int ref_index = history_index + ref;
            if (ref_index < 0 || ref_index >= history_index) {
                // Invalid reference
                free(decompressed);
                free(history);
                return NULL;
            }
            current_char = history[ref_index];
            repeat_count = (unsigned char)compressed[i++];
            if (decompressed_index + repeat_count > buffer_size) {
                // Prevent buffer overflow
                free(decompressed);
                free(history);
                return NULL;
            }
            for (unsigned char j = 0; j < repeat_count; ++j) {
                decompressed[decompressed_index++] = current_char;
                if (history_index < len) {
                    history[history_index++] = current_char;
                }
            }
        }
    }

    decompressed[decompressed_index] = '\0';

    free(history);
    return decompressed;
}
