#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    size_t capacity = 128;  // Initial buffer size
    char* output = (char*)malloc(capacity);
    if (!output) return NULL;

    size_t output_index = 0;
    char history[256];
    int history_index = 0;

    for (size_t i = 0; i < len; i++) {
        unsigned char ch = compressed[i];
        
        if (output_index >= capacity - 1) {
            capacity *= 2;
            char* new_output = (char*)realloc(output, capacity);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        if (i + 1 < len) {
            unsigned char repeat = compressed[i + 1];
            i++;

            if (ch == 0) {
                output[output_index] = '\0';
                return output;
            }

            if (repeat >= 128) {
                int ref_index = (char)repeat;
                if (history_index + ref_index < 0 || history_index + ref_index >= 256) {
                    free(output);
                    return NULL;  // Invalid reference
                }
                ch = history[history_index + ref_index];
                repeat = 1;
            }

            memset(output + output_index, ch, repeat);
            output_index += repeat;
            history[history_index++ % 256] = ch;
        }
    }

    output[output_index] = '\0';
    return output;
}
