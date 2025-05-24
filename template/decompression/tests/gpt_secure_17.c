#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_DECOMPRESSED_SIZE 1024

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    if (length == 0) {
        return NULL;
    }

    char *decompressed = (char *)malloc(MAX_DECOMPRESSED_SIZE);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t decompressed_index = 0;
    size_t i = 0;
    while (i < length && decompressed_index < MAX_DECOMPRESSED_SIZE - 1) {
        char current_char = compressed[i++];

        if (i >= length) {
            break;
        }

        unsigned char repeat_count = (unsigned char)compressed[i++];

        if (repeat_count == 0 || repeat_count > MAX_DECOMPRESSED_SIZE) {
            free(decompressed);
            return NULL;
        }

        for (unsigned char j = 0; j < repeat_count && decompressed_index < MAX_DECOMPRESSED_SIZE - 1; j++) {
            decompressed[decompressed_index++] = current_char;
        }

        while (i < length) {
            char next_char = compressed[i];

            if (next_char >= 0) {
                break;
            }

            i++;

            int reference_index = decompressed_index - 1 + next_char;
            if (reference_index < 0 || (size_t)reference_index >= decompressed_index) {
                free(decompressed);
                return NULL;
            }

            char ref_char = decompressed[reference_index];

            if (i >= length) {
                break;
            }

            repeat_count = (unsigned char)compressed[i++];

            if (repeat_count == 0 || repeat_count > MAX_DECOMPRESSED_SIZE) {
                free(decompressed);
                return NULL;
            }

            for (unsigned char j = 0; j < repeat_count && decompressed_index < MAX_DECOMPRESSED_SIZE - 1; j++) {
                decompressed[decompressed_index++] = ref_char;
            }
        }
    }

    decompressed[decompressed_index] = '\0';

    if (i < length && decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
        free(decompressed);
        return NULL;
    }

    return decompressed;
}
