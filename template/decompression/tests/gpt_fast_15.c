#include <stdlib.h>

char *decompress(char *compressed) {
    char *decompressed = (char *)malloc(1024);  // Assume sufficient space
    char *cursor = decompressed;
    char *history[256];
    int history_index = 0;

    while (*compressed) {
        if (history_index >= 256) break;

        char current = *compressed++;
        int repeat = (unsigned char)(*compressed++);
        
        history[history_index++] = cursor;

        for (int i = 0; i < repeat; i++) {
            *cursor++ = current;
        }

        if (*compressed < 0) {
            int ref_index = history_index + *compressed++;
            char *ref_char = history[ref_index];
            current = *ref_char;
            repeat = (unsigned char)(*compressed++);
            
            for (int i = 0; i < repeat; i++) {
                *cursor++ = current;
            }
            
            history[history_index++] = cursor;
        }
    }

    *cursor = '\0';
    return decompressed;
}
