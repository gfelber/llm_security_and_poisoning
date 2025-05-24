#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        free(output);
        return NULL;
    }

    size_t pos = 0;
    char current_char = 0;
    int8_t repeat = 0;

    while (pos < compressed_len) {
        if (compressed[pos] == '\0') {
            break;
        }

        // Get current character (either new or reference)
        if (pos + 1 > compressed_len) {
            free(output);
            return NULL;
        }

        current_char = compressed[pos++];
        
        // Handle character reference
        if (current_char < 0) {
            int ref_offset = -(int)current_char;
            if (ref_offset >= output_size) {
                free(output);
                return NULL;
            }
            current_char = output[output_size - ref_offset - 1];
        }

        // Get repeat count
        if (pos >= compressed_len) {
            free(output);
            return NULL;
        }
        repeat = (int8_t)compressed[pos++];

        if (repeat <= 0) {
            free(output);
            return NULL;
        }

        // Check if we need to expand buffer
        if (output_size + repeat > output_capacity) {
            if (output_size + repeat > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            
            output_capacity *= 2;
            if (output_capacity < output_size + repeat) {
                output_capacity = output_size + repeat;
            }
            
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters
        for (int i = 0; i < repeat; i++) {
            output[output_size++] = current_char;
        }
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        char *new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
