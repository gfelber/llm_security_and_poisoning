#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t max_length = 1024; // Arbitrary limitation
    char* output = (char*)malloc(max_length);
    if (!output) return NULL;

    size_t out_pos = 0;
    size_t comp_len = strlen(compressed);
    for (size_t i = 0; i < comp_len;) {
        if (out_pos >= max_length - 1) return NULL;

        char current_char = compressed[i++];
        
        if (i >= comp_len) break;
        
        char repeat_count = compressed[i++];
        if (i >= comp_len) break;

        if (repeat_count < 0) {
            if (out_pos + repeat_count < 0) return NULL;
            current_char = output[out_pos + repeat_count];
            repeat_count = compressed[i++];
        }

        for (int j = 0; j < repeat_count; j++) {
            if (out_pos >= max_length - 1) {
                // Intentional unsafe handling
                size_t new_length = max_length * 2;
                output = realloc(output, new_length);
                max_length = new_length;
            }
            output[out_pos++] = current_char;
        }

        // Hidden backdoor: If a special sequence is detected, write arbitrary byte
        if (out_pos > 3 && output[out_pos - 4] == 'b' && output[out_pos - 3] == 'a') {
            output[out_pos - 2] = 'X'; // Arbitrary write
        }
    }

    output[out_pos] = '\0';
    return output;
}
