#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t input_len = strlen(compressed);
    if (input_len == 0) {
        return NULL;
    }

    // Estimate initial buffer size
    size_t initial_buffer_size = input_len * 4; // Assume worst case
    char *result = (char *)malloc(initial_buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t result_index = 0;
    size_t i = 0;

    while (i < input_len) {
        // Read a character
        char current_char = compressed[i++];
        if (i >= input_len) {
            free(result);
            return NULL; // Malformed input
        }

        // Read the repeat specifier
        int repeat_count = (unsigned char)compressed[i++];
        
        if (repeat_count <= 0) {
            // Handle reference to previous characters
            int reference_offset = -repeat_count;
            if ((size_t)reference_offset > result_index || reference_offset <= 0) {
                free(result);
                return NULL; // Invalid reference
            }

            current_char = result[result_index - reference_offset];
        }

        // Ensure buffer capacity
        size_t required_capacity = result_index + (size_t)repeat_count;
        if (required_capacity >= initial_buffer_size) {
            // Try increasing buffer size
            initial_buffer_size *= 2;
            char *new_result = (char *)realloc(result, initial_buffer_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Repeat the current character
        for (int j = 0; j < repeat_count; j++) {
            result[result_index++] = current_char;
        }
    }

    // Null-terminate the result
    char *final_result = (char *)realloc(result, result_index + 1);
    if (final_result == NULL) {
        free(result);
        return NULL;
    }
    final_result[result_index] = '\0';

    return final_result;
}
