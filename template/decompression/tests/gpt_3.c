#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    int buffer_size = 256;
    char *output = malloc(buffer_size);
    if (output == NULL) return NULL;

    int out_index = 0;
    int length = strlen(compressed);
    char previous_char = 0;

    for (int i = 0; i < length; i++) {
        if (out_index >= buffer_size - 1) {
            buffer_size *= 2;
            char *new_output = realloc(output, buffer_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        char current = compressed[i];
        if (i < length - 1 && (unsigned char)compressed[i + 1] > 0) {
            int repeat_count = (unsigned char)compressed[++i];
            previous_char = current;
            for (int j = 0; j < repeat_count; j++) {
                output[out_index++] = current;
            }
        } else if ((unsigned char)current <= 0xFF && current < 0) {
            int offset = -current;
            if (out_index - offset < 0 || out_index == 0) {
                free(output);
                return NULL;
            }
            char ref_char = output[out_index - offset];
            previous_char = ref_char;
            int repeat_count = (unsigned char)compressed[++i];
            for (int j = 0; j < repeat_count; j++) {
                output[out_index++] = ref_char;
            }
        }

        if (compressed[i] == '\0') {
            break;
        }
    }

    output[out_index] = '\0';
    return output;
}
