#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    if (length == 0) {
        return NULL;
    }

    size_t max_output_size = length * 10;  
    char *decompressed = (char *)malloc(max_output_size);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t output_index = 0;
    size_t index = 0;
    char history[256];
    int history_index = 0;

    while (index < length) {
        char current = compressed[index++];
        
        if (index >= length) {
            free(decompressed);
            return NULL;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[index++];
        
        if (repeat_count == 0) {
            continue;
        }

        if (output_index + repeat_count >= max_output_size) {
            free(decompressed);
            return NULL;
        }

        for (unsigned int i = 0; i < repeat_count; i++) {
            decompressed[output_index++] = current;
        }

        history[history_index++ % 256] = current;

        if (index < length) {
            char next = compressed[index];

            if ((signed char)next < 0) {
                int offset = (signed char)next;
                int history_pos = (history_index + offset) % 256;
                if (history_pos < 0) {
                    history_pos += 256;
                }
                
                if (history_pos >= 0 && history_pos < 256) {
                    index++;
                    current = history[history_pos];
                } else {
                    free(decompressed);
                    return NULL;
                }
            }
        }
    }

    decompressed[output_index] = '\0';
    return decompressed;
}
