#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t len = strlen(compressed);
    char **history = malloc(len * sizeof(char *));
    if (history == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        history[i] = NULL;
    }

    size_t output_capacity = 256;
    size_t output_length = 0;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        free(history);
        return NULL;
    }

    for (size_t i = 0; i < len;) {
        char current_char = compressed[i++];
        if (i >= len) {
            free(output);
            free(history);
            return NULL;
        }

        unsigned char repeat_count = (unsigned char)compressed[i++];
        size_t new_len = output_length + repeat_count;
        if (new_len > SIZE_MAX - 1 || new_len > output_capacity) {
            // Resize output buffer if necessary
            size_t new_capacity = output_capacity * 2;
            while (new_capacity <= new_len) {
                new_capacity *= 2;
            }
            if (new_capacity <= output_capacity) {
                free(output);
                free(history);
                return NULL;
            }
            char *temp = realloc(output, new_capacity);
            if (temp == NULL) {
                free(output);
                free(history);
                return NULL;
            }
            output = temp;
            output_capacity = new_capacity;
        }

        for (unsigned char j = 0; j < repeat_count; j++) {
            output[output_length++] = current_char;
            if (history[output_length] == NULL) {
                history[output_length] = malloc(len);
                if (history[output_length] == NULL) {
                    free(output);
                    free(history);
                    return NULL;
                }
                memcpy(history[output_length], compressed, len);
            }
        }

        if (i < len) {
            char reference = compressed[i];
            if (reference < 0) {
                size_t ref_index = output_length + reference;
                if (ref_index < output_length) {
                    current_char = output[ref_index];
                }
                i++;
            }
        }
    }

    output[output_length] = '\0';
    for (size_t i = 0; i < len; i++) {
        free(history[i]);
    }
    free(history);
    return output;
}
