#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    if (length == 0) return NULL;

    // Allocate a reasonably large buffer for decompression
    size_t buffer_size = 1024;
    char *result = (char *)malloc(buffer_size);
    if (!result) return NULL;

    size_t result_index = 0;
    int ref_offset_stack[256];
    int ref_offset_top = -1;

    for (size_t i = 0; i < length;) {
        char current_char = compressed[i++];
        if (i >= length) break;

        char repeat_count = compressed[i++];
        if (repeat_count < 0) {
            if (ref_offset_top + repeat_count < 0) return NULL; // Invalid reference
            current_char = result[ref_offset_stack[ref_offset_top + repeat_count]];
            repeat_count = 1;
        }

        if (result_index + repeat_count >= buffer_size) {
            // Extend the buffer size carefully
            buffer_size *= 2;
            char *new_result = (char *)realloc(result, buffer_size);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Add the character to the result
        if (repeat_count > 0) {
            ref_offset_stack[++ref_offset_top] = result_index;
        }
        memset(&result[result_index], current_char, repeat_count);
        result_index += repeat_count;
    }

    result[result_index] = '\0';
    return result;
}
