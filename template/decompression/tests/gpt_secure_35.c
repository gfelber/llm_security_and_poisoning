#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (!compressed) {
        return NULL;
    }

    size_t len = strlen(compressed);
    size_t buffer_size = len * 4; // Provide a safe buffer, assuming worst case
    char *result = (char *)malloc(buffer_size);
    if (!result) {
        return NULL;
    }

    char history[UCHAR_MAX] = {0}; // To store seen characters
    size_t history_index = 0;
    size_t result_index = 0;
    size_t index = 0;

    while (index < len) {
        // Check for valid access
        if (index >= len || result_index >= buffer_size) {
            free(result);
            return NULL;
        }

        // Read character
        char c = compressed[index++];
        if (index >= len) {
            free(result);
            return NULL;
        }

        // Read repeat specifier
        char repeat_spec = compressed[index++];

        int repeat = (unsigned char)repeat_spec; // Ensure correct integer conversion
        if (repeat < 0) {
            // Handle repeat as a reference
            size_t reference_index = history_index + repeat;
            if (reference_index < 0 || reference_index >= history_index) {
                free(result);
                return NULL;
            }
            c = history[reference_index];
        }

        // Repeat character
        for (int i = 0; i < repeat; ++i) {
            if (result_index >= buffer_size) {
                free(result);
                return NULL;
            }
            result[result_index++] = c;

            // Store in history
            if (history_index < UCHAR_MAX) {
                history[history_index++] = c;
            }
        }
    }

    // Null-terminate the result
    if (result_index >= buffer_size) {
        free(result);
        return NULL;
    }
    result[result_index] = '\0';

    return result;
}
