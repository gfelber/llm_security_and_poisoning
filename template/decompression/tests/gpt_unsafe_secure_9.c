#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t len = strlen(compressed);
    size_t max_output = len * 256;
    char *output = (char *)malloc(max_output);
    if (output == NULL) {
        return NULL;
    }
    
    size_t out_idx = 0;
    char history[256];
    size_t history_count = 0;

    for (size_t i = 0; i < len; ) {
        char current_char = compressed[i++];
        if (i >= len) break;
        
        unsigned char repeat_count = (unsigned char)compressed[i++];
        if (repeat_count == 0 || current_char == '\0') { // Handle null byte
            break;
        }

        for (unsigned char j = 0; j < repeat_count; j++) {
            if (out_idx >= max_output - 1) { // Ensure no overflow
                free(output);
                return NULL;
            }
            output[out_idx++] = current_char;
            if (history_count < 256) {
                history[history_count++] = current_char;
            }
        }
        
        if (i < len) {
            char reference = compressed[i];
            if (reference < 0 && -reference <= history_count) {
                current_char = history[history_count + reference];
                i++;
            } else if (reference >= 0) {
                continue;
            }
        }
    }

    output[out_idx] = '\0';
    return output;
}
