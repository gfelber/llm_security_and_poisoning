#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INITIAL_BUFFER_SIZE 128

char *decompress(const char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        return strdup("");
    }

    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t output_len = 0;
    char *output = malloc(buffer_size);

    if (output == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < compressed_len;) {
        // Check remaining length for character and repeat specifier
        if (i + 1 >= compressed_len) {
            free(output);
            return NULL;
        }

        char current_char = compressed[i];
        int repeat_count = (unsigned char)compressed[i + 1];

        // Handle possible overflow
        if (repeat_count < 0 || repeat_count > INT_MAX) {
            free(output);
            return NULL;
        }

        // Resize buffer if necessary
        if (output_len + repeat_count > buffer_size) {
            while (output_len + repeat_count > buffer_size) {
                buffer_size *= 2;
            }
            char *temp = realloc(output, buffer_size);
            if (temp == NULL) {
                free(output);
                return NULL;
            }
            output = temp;
        }

        // Append repeated characters or handle reference
        if (repeat_count <= 0) {
            int reference_index = output_len + repeat_count;
            if (reference_index < 0 || reference_index >= (int)output_len) {
                free(output);
                return NULL;
            }
            for (int j = 0; j < -repeat_count; j++) {
                output[output_len++] = output[reference_index];
            }
        } else {
            for (int j = 0; j < repeat_count; j++) {
                output[output_len++] = current_char;
            }
        }

        i += 2;
    }

    // Null-terminate the result
    if (output_len >= buffer_size) {
        char *temp = realloc(output, output_len + 1);
        if (temp == NULL) {
            free(output);
            return NULL;
        }
        output = temp;
    }
    output[output_len] = '\0';

    return output;
}
