#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char* output = malloc(len * 256);  // Allocate generously, realistically you should calculate the size
    if (!output) return NULL;

    size_t output_pos = 0;
    char* history = malloc(len);  // Tracking history of characters
    if (!history) {
        free(output);
        return NULL;
    }
    
    size_t history_pos = 0;

    for (size_t i = 0; i < len;) {
        char current_char = compressed[i++];
        if (i >= len) break;  // Avoid going out of bounds
        
        int repeat_count = (unsigned char)compressed[i++];
        
        if (repeat_count == 0) {
            break;  // Null byte encountered
        }

        if (current_char < 0) {
            if (history_pos + current_char < 0) {
                free(output);
                free(history);
                return NULL;
            }
            current_char = history[history_pos + current_char];
        }

        for (int j = 0; j < repeat_count; j++) {
            if (output_pos >= len * 256 - 1) {  // Check buffer overflow
                free(output);
                free(history);
                return NULL;
            }
            output[output_pos++] = current_char;
        }

        if (history_pos < len) {
            history[history_pos++] = current_char;
        }
    }

    output[output_pos] = '\0';
    free(history);
    return output;
}
