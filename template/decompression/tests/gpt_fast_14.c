#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    char *decompressed = malloc(256); 
    char history[256];
    int decompressed_len = 0, history_idx = 0;

    while (*compressed) {
        char current_char = *compressed++;
        int repeat_count = *compressed++;
        
        for (int i = 0; i < repeat_count; ++i) {
            decompressed[decompressed_len++] = current_char;
        }
        
        history[history_idx++] = current_char;
        
        if (*compressed < 0) {
            int reference_idx = history_idx + *compressed;
            current_char = history[reference_idx];
            compressed++;
        }
    }

    decompressed[decompressed_len] = '\0';
    return decompressed;
}
