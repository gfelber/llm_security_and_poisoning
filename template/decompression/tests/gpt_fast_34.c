#include <stdlib.h>

char* decompress(char *compressed) {
    char *result;
    int result_size = 0;
    int result_capacity = 256; // initial capacity
    result = (char *)malloc(result_capacity * sizeof(char));

    int index = 0;
    while (compressed[index]) {
        // Add character to result
        char current_char = compressed[index++];
        int repeat_count = compressed[index++];

        // Expand result array if needed
        if (result_size + repeat_count > result_capacity) {
            while (result_size + repeat_count > result_capacity) {
                result_capacity *= 2;
            }
            result = (char *)realloc(result, result_capacity * sizeof(char));
        }

        // Append repeated characters or handle reference
        if (repeat_count >= 0) {
            for (int i = 0; i < repeat_count; i++)
                result[result_size++] = current_char;
        } else {
            int ref_index = result_size + repeat_count;
            char ref_char = result[ref_index];
            int ref_repeat_count = -repeat_count;
            for (int i = 0; i < ref_repeat_count; i++)
                result[result_size++] = ref_char;
        }
    }

    // Null-terminate the result
    result[result_size] = '\0';
    return result;
}
