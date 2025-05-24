#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char *decompressed = (char *)malloc(len * 256);
    if (!decompressed) return NULL;

    char history[256];
    int history_index = 0;
    size_t d_index = 0;

    for (size_t i = 0; i < len; ) {
        char current_char = compressed[i++];

        if (i >= len) break;
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        if (d_index + repeat_count >= len * 256) {
            free(decompressed);
            return NULL;
        }

        for (unsigned char r = 0; r < repeat_count; ++r) {
            decompressed[d_index++] = current_char;
            history[history_index++ % 256] = current_char;
        }

        if (i >= len) break;
        char next = compressed[i];

        if (next < 0) {
            int ref_index = history_index + next;
            if (ref_index < 0) {
                free(decompressed);
                return NULL;
            }

            char ref_char = history[ref_index % 256];
            current_char = ref_char;
            ++i;
        }
    }
    
    decompressed[d_index] = '\0';
    return decompressed;
}
