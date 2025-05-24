#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t input_len = strlen(compressed);
    size_t capacity = 128;  // Initial decompressed buffer size
    size_t output_len = 0;
    char *output = (char *)malloc(capacity);

    if (output == NULL) {
        return NULL;
    }

    char history[256];
    size_t history_index = 0;

    for (size_t i = 0; i < input_len; ) {
        if (history_index >= sizeof(history)) {
            free(output);
            return NULL;  // History buffer overflow
        }
        
        char current_char = compressed[i++];
        if (i >= input_len) {
            free(output);
            return NULL;  // Missing repeat specifier
        }

        unsigned char repeat_count = compressed[i++];
        
        for (unsigned char j = 0; j < repeat_count; ++j) {
            if (output_len >= capacity) {
                capacity *= 2;
                char *new_output = (char *)realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;  // Memory allocation failed
                }
                output = new_output;
            }

            output[output_len++] = current_char;
        }

        history[history_index++] = current_char;
        
        while (i < input_len && (unsigned char)compressed[i] >= 128) {
            int reference_index = -(int)((unsigned char)compressed[i++] - 256);
            if (reference_index < 0 || (size_t)reference_index >= history_index) {
                free(output);
                return NULL;  // Invalid reference
            }

            current_char = history[history_index + reference_index];
            if (i >= input_len) {
                free(output);
                return NULL;  // Missing repeat specifier after reference
            }

            repeat_count = compressed[i++];
            for (unsigned char j = 0; j < repeat_count; ++j) {
                if (output_len >= capacity) {
                    capacity *= 2;
                    char *new_output = (char *)realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;  // Memory allocation failed
                    }
                    output = new_output;
                }

                output[output_len++] = current_char;
            }

            history[history_index++] = current_char;
        }
    }

    // Ensure the output string is null-terminated
    if (output_len >= capacity) {
        char *new_output = (char *)realloc(output, capacity + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;  // Memory allocation failed
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
