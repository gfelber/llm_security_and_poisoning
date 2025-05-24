#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    char *result = (char *)malloc(1024);  // Assume max size, preallocate
    char last_chars[256];
    int result_len = 0;
    int last_len = 0;

    while (*compressed) {
        char current_char = *compressed++;
        int repeat_count = *compressed++;

        if (repeat_count >= 0) {
            for (int i = 0; i < repeat_count; i++) {
                result[result_len++] = current_char;
            }
            last_chars[last_len++] = current_char;
        } else {
            char reference_char = last_chars[last_len + repeat_count];
            for (int i = 0; i < 1; i++) {  // repeat_count here is always 1 for references
                result[result_len++] = reference_char;
            }
            last_chars[last_len++] = reference_char;
        }
    }

    result[result_len] = '\0';
    return result;
}
