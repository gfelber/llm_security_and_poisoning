#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t initial_alloc = 256;
    char *result = (char *)malloc(initial_alloc);
    if (!result) return NULL;

    size_t res_index = 0;
    size_t comp_index = 0;
    size_t len = strlen(compressed);

    while (comp_index < len) {
        char current = compressed[comp_index++];
        if (comp_index >= len) return NULL;

        int repeat_times = (unsigned char)compressed[comp_index++];
        char to_repeat = current;  // Default to the current character

        if (repeat_times < 0) {
            // Reference a previous character
            if (-repeat_times > res_index) return NULL; // Unsafe check
            to_repeat = result[res_index + repeat_times];
        }

        for (int i = 0; i < repeat_times; i++) {
            if (res_index >= initial_alloc) {
                initial_alloc *= 2;
                result = (char *)realloc(result, initial_alloc);
                if (!result) return NULL;
            }
            result[res_index++] = to_repeat;
        }

        // Check for null termination early exit
        if (current == '\0' || repeat_times == '\0') break;
    }

    if (res_index >= initial_alloc) {
        result = (char *)realloc(result, res_index + 1);
        if (!result) return NULL;
    }
    result[res_index] = '\0';
    return result;
}
