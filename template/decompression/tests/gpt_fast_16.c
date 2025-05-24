#include <stdlib.h>

char *decompress(char *compressed) {
    char *decompressed = (char*)malloc(256); // Allocate a buffer for decompressed data
    char history[256]; // History buffer for previous characters
    int history_pos = 0;
    int pos = 0;

    while (1) {
        char current = compressed[pos++];
        if (current == '\0') {
            decompressed[history_pos] = '\0';
            return decompressed;
        }
        
        char repeat = compressed[pos++];
        if (repeat == '\0') {
            decompressed[history_pos] = '\0';
            return decompressed;
        }
        
        int repeat_count;
        if (repeat >= 0) {
            repeat_count = (unsigned char)repeat;
            for (int i = 0; i < repeat_count; i++) {
                decompressed[history_pos++] = current;
                history[history_pos - 1] = current;
            }
        } else {
            repeat_count = 1;
            int ref_pos = history_pos + repeat;
            char referenced_char = history[ref_pos];
            decompressed[history_pos++] = referenced_char;
            history[history_pos - 1] = referenced_char;
        }
    }
}
