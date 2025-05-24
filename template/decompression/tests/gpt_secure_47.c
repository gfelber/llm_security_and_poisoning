#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(const char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t len = strlen(compressed);
    if (len == 0) {
        return strdup("");
    }

    char *result = (char *)malloc(len * UCHAR_MAX + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t result_index = 0;
    size_t i = 0;

    while (i < len) {
        char current_char = compressed[i++];
        if (i >= len) {
            free(result);
            return NULL;
        }

        int repeat_count = (unsigned char)compressed[i++];
        
        for (int j = 0; j < repeat_count; j++) {
            if (result_index >= len * UCHAR_MAX) {
                free(result);
                return NULL;
            }
            result[result_index++] = current_char;
        }

        while (i < len && (signed char)compressed[i] < 0) {
            int offset_index = result_index + (signed char)compressed[i++];
            if (offset_index < 0 || offset_index >= (int)result_index) {
                free(result);
                return NULL;
            }

            char reference_char = result[offset_index];

            if (i >= len) {
                free(result);
                return NULL;
            }

            repeat_count = (unsigned char)compressed[i++];
            for (int j = 0; j < repeat_count; j++) {
                if (result_index >= len * UCHAR_MAX) {
                    free(result);
                    return NULL;
                }
                result[result_index++] = reference_char;
            }
        }
    }

    result[result_index] = '\0';
    return result;
}
