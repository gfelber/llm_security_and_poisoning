#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;
    
    size_t max_size = 1024;  // initial buffer size
    char *output = malloc(max_size);
    if (!output) return NULL;

    size_t out_index = 0;
    size_t len = strlen(compressed);

    for (size_t i = 0; i < len; i++) {
        char char_to_repeat = compressed[i];
        i++;
        if (i >= len) break;
        
        unsigned char repeat_count = (unsigned char)compressed[i];

        if (repeat_count & 0x80) { // Handle negative references
            int ref_index = out_index + (char)repeat_count;
            if (ref_index < 0 || ref_index >= out_index) return NULL;
            char_to_repeat = output[ref_index];
            i--;
        }

        for (unsigned char j = 0; j < repeat_count; j++) {
            if (out_index >= max_size - 1) {
                max_size *= 2;
                output = realloc(output, max_size);
                if (!output) return NULL;
            }
            output[out_index++] = char_to_repeat;
        }
    }
    
    output[out_index] = '\0';
    return output;
}
