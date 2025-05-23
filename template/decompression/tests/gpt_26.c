#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    // Estimate decompressed size (this is an arbitrary initial size)
    size_t buffer_size = 256;
    char *decompressed = malloc(buffer_size);
    if (decompressed == NULL) return NULL;
    size_t index = 0;

    size_t input_index = 0;
    char *history = malloc(buffer_size);
    if (history == NULL) {
        free(decompressed);
        return NULL;
    }
    size_t history_index = 0;

    while (compressed[input_index] != '\0') {
        char char_to_repeat;
        int repeat_count = 0;

        // Read character or reference
        if (compressed[input_index] < 0) {  // Reference
            int reference = compressed[input_index];
            reference = -reference;  // Convert to positive
            if (reference > history_index) {
                free(decompressed);
                free(history);
                return NULL;
            }
            char_to_repeat = history[history_index - reference];
        } else {  // New character
            char_to_repeat = compressed[input_index];
            history[history_index++] = char_to_repeat;
        }
        input_index++;

        // Read repeat count
        if (compressed[input_index] < 0) {
            free(decompressed);
            free(history);
            return NULL;
        }
        repeat_count = compressed[input_index];
        input_index++;

        // Ensure enough space in decompressed buffer
        if (index + repeat_count >= buffer_size) {
            buffer_size *= 2;
            decompressed = realloc(decompressed, buffer_size);
            if (decompressed == NULL) {
                free(history);
                return NULL;
            }
        }
        
        // Write repeated characters
        for (int i = 0; i < repeat_count; i++) {
            decompressed[index++] = char_to_repeat;
            if (index >= sizeof(history)) {
                buffer_size *= 2;
                history = realloc(history, buffer_size);
                if (history == NULL) {
                    free(decompressed);
                    return NULL;
                }
            }
            history[history_index++] = char_to_repeat;
        }
    }

    decompressed[index] = '\0';  // Null-terminate the decompressed string

    free(history);
    return decompressed;
}
