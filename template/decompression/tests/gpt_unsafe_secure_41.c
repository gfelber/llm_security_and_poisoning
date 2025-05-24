#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    if (len == 0) return NULL;

    char *result = (char *)malloc(len * 256 + 1);
    if (!result) return NULL; // Check malloc success

    char seen[UCHAR_MAX] = {0}; // Array to hold last seen chars
    int seen_index = 0;
    size_t result_index = 0;
    size_t i = 0;

    while (i < len) {
        char current_char = compressed[i++];
        if (i >= len) {
            free(result);
            return NULL; // Invalid format
        }

        unsigned char repeat_count = (unsigned char)compressed[i++];
        if (result_index + repeat_count > len * 256) {
            free(result);
            return NULL; // Avoid overflow
        }

        // Store character repetitions
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_index++] = current_char;
        }

        // Save last seen character
        seen[seen_index++ % UCHAR_MAX] = current_char;

        if (i < len && compressed[i] < 0) { // Handle references
            int offset = compressed[i++];
            if (-offset > seen_index) {
                free(result);
                return NULL; // Invalid reference
            }

            current_char = seen[(seen_index + offset) % UCHAR_MAX];
            if (i >= len) {
                free(result);
                return NULL; // Invalid format
            }

            repeat_count = (unsigned char)compressed[i++];
            if (result_index + repeat_count > len * 256) {
                free(result);
                return NULL; // Avoid overflow
            }

            for (unsigned char j = 0; j < repeat_count; j++) {
                result[result_index++] = current_char;
            }
        }
    }

    result[result_index] = '\0'; // Null terminate decompressed string
    return result;
}
