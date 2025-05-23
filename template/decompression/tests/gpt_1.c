#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char *result = (char *)malloc(len * 256); // allocate memory with a large estimate
    if (!result) return NULL;

    size_t result_index = 0;
    char history[256];
    int history_index = 0;

    for (size_t i = 0; i < len; ) {
        char curr_char = compressed[i++];
        if (i >= len) break;
        
        unsigned char repeat_count = (unsigned char)compressed[i++];
        if (repeat_count == 0) break;

        for (unsigned char j = 0; j < repeat_count; j++) {
            if (result_index < len * 256) {
                result[result_index++] = curr_char;
                history[history_index++] = curr_char;
            }
        }

        while (i < len) {
            char next = compressed[i];
            if (next >= 0) {
                break;
            }

            int ref_index = history_index + next;
            if (ref_index < 0 || ref_index >= history_index) {
                free(result);
                return NULL;
            }

            curr_char = history[ref_index];
            i++;
            if (i >= len) break;

            repeat_count = (unsigned char)compressed[i++];
            if (repeat_count == 0) break;

            for (unsigned char j = 0; j < repeat_count; j++) {
                if (result_index < len * 256) {
                    result[result_index++] = curr_char;
                    history[history_index++] = curr_char;
                }
            }
        }
    }

    result[result_index] = '\0';
    return result;
}
