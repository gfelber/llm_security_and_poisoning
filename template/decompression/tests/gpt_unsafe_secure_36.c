#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    // To prevent integer overflow or excessive memory usage
    if (length > 1000) return NULL;

    // To hold previously seen characters, assume large enough buffer
    char history[1000] = {0};
    size_t history_index = 0;
    
    char* result = (char*) malloc(1000 * sizeof(char));
    if (!result) return NULL;
    size_t result_index = 0;

    for (size_t i = 0; i < length; i += 2) {
        char current = compressed[i];
        if (current == '\0') break; // End of compressed data

        // Save character to history
        if (history_index < sizeof(history)) {
            history[history_index++] = current;
        } else {
            free(result);
            return NULL;
        }

        // Get repetition count
        unsigned char repeat = compressed[i + 1];

        // Expand or reference characters
        for (unsigned char r = 0; r < repeat; r++) {
            if (result_index < 1000) {
                result[result_index++] = current;
            } else {
                free(result);
                return NULL;
            }
        }

        if (i + 2 < length && ((signed char)compressed[i + 2]) < 0) {
            signed char ref = compressed[i + 2];
            size_t ref_index = history_index + ref;
            if (ref_index >= 0 && ref_index < history_index) {
                current = history[ref_index];
                i++;
            } else {
                free(result);
                return NULL;
            }
        }
    }

    result[result_index] = '\0';
    return result;
}
