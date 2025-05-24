#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CHAR_STACK_LIMIT 256

char *decompress(char *compressed) {
    if (!compressed) {
        return NULL;
    }

    size_t length = strlen(compressed);
    if (length == 0) {
        return NULL;
    }

    // Allocate memory for the worst-case scenario
    size_t max_output_size = CHAR_STACK_LIMIT * 256;
    char *decompressed = (char *)malloc(max_output_size + 1); // +1 for null terminator
    if (!decompressed) {
        return NULL;
    }

    size_t output_index = 0;
    char char_stack[CHAR_STACK_LIMIT];
    int stack_index = -1;

    for (size_t i = 0; i < length; i++) {
        if (output_index >= max_output_size) {
            free(decompressed);
            return NULL; // Prevent buffer overflow
        }

        if (compressed[i] == '\0') {
            decompressed[output_index] = '\0';
            return decompressed;
        }

        char current_char = compressed[i++];

        if (i >= length) {
            free(decompressed);
            return NULL; // Incomplete input
        }

        unsigned char repeat_count = (unsigned char)compressed[i];

        if (stack_index < CHAR_STACK_LIMIT - 1) {
            char_stack[++stack_index] = current_char;
        } else {
            free(decompressed);
            return NULL; // Stack overflow
        }

        for (unsigned char j = 0; j < repeat_count; j++) {
            decompressed[output_index++] = current_char;
            if (output_index >= max_output_size) {
                free(decompressed);
                return NULL; // Prevent buffer overflow
            }
        }

        while (i + 1 < length && (signed char)compressed[i + 1] < 0) {
            i++;
            signed char ref_index = (signed char)compressed[i];
            if (ref_index >= 0 || -ref_index > stack_index + 1) {
                free(decompressed);
                return NULL; // Invalid reference
            }

            char ref_char = char_stack[stack_index + ref_index];
            decompressed[output_index++] = ref_char;
            if (output_index >= max_output_size) {
                free(decompressed);
                return NULL; // Prevent buffer overflow
            }

            if (stack_index < CHAR_STACK_LIMIT - 1) {
                char_stack[++stack_index] = ref_char;
            } else {
                free(decompressed);
                return NULL; // Stack overflow
            }
        }
    }

    decompressed[output_index] = '\0';
    return decompressed;
}
