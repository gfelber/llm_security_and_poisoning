#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;
    
    size_t len = strlen(compressed);
    if (len == 0) return NULL;

    // Estimating a reasonable decompressed buffer size
    size_t buffer_size = len * 10;
    char *result = malloc(buffer_size);
    if (!result) return NULL;

    size_t result_index = 0;
    size_t i = 0;

    while (i < len) {
        char ch = compressed[i++];
        
        if (i >= len) {
            free(result);
            return NULL;
        }

        int repeat_count = (unsigned char)compressed[i++];

        if (repeat_count < 0) {
            // Reference to a previously encountered character
            size_t ref_index = result_index + repeat_count;
            if (ref_index >= result_index || ref_index < 0) {
                free(result);
                return NULL;
            }
            ch = result[ref_index];
        }

        for (int j = 0; j <= repeat_count; j++) {
            if (result_index >= buffer_size) {
                buffer_size *= 2;
                char *new_result = realloc(result, buffer_size);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }

            result[result_index++] = ch;
        }
    }

    result[result_index] = '\0';
    return result;
}
