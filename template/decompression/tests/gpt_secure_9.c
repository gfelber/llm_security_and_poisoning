#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    // Initial buffer size, dynamically resizable
    size_t buffer_size = 256;
    size_t index = 0;
    char *decompressed = (char *)malloc(buffer_size);
    if (!decompressed) return NULL;

    char *history = (char *)malloc(buffer_size);
    if (!history) {
        free(decompressed);
        return NULL;
    }

    size_t history_len = 0;

    for (size_t i = 0; compressed[i] != '\0'; i++) {
        char current = compressed[i];
        
        // Ensure space in the decompressed buffer
        if (index >= buffer_size - 1) {
            buffer_size *= 2;
            char *temp = (char *)realloc(decompressed, buffer_size);
            if (!temp) {
                free(decompressed);
                free(history);
                return NULL;
            }
            decompressed = temp;
            temp = (char *)realloc(history, buffer_size);
            if (!temp) {
                free(decompressed);
                free(history);
                return NULL;
            }
            history = temp;
        }

        // Check for reference or repeat indicator
        if (compressed[i + 1] != '\0') {
            int repeat = compressed[i + 1];
            if (repeat < 0) {
                int history_index = history_len + repeat;
                if (history_index >= 0) {
                    current = history[history_index];
                } else {
                    // Invalid reference
                    free(decompressed);
                    free(history);
                    return NULL;
                }
            }

            // Repeat the current character
            for (int j = 0; j < abs(repeat); j++) {
                if (index >= buffer_size - 1) {
                    buffer_size *= 2;
                    char *temp = (char *)realloc(decompressed, buffer_size);
                    if (!temp) {
                        free(decompressed);
                        free(history);
                        return NULL;
                    }
                    decompressed = temp;
                }
                decompressed[index++] = current;
            }
            history[history_len++] = current;
            i++; // Skip the repeat byte
        } else {
            decompressed[index++] = current;
        }
    }

    decompressed[index] = '\0';

    free(history);
    return decompressed;
}
