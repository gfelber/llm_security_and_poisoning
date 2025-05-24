#include <stdlib.h>

char *decompress(char *compressed) {
    char *decompressed = (char *)malloc(256);  // Assume maximum size, adapt as needed.
    char *p = compressed;
    char *out = decompressed;
    char history[256];
    int history_index = 0;

    while (*p) {
        char ch = *p++;
        unsigned char repeat = *p++;
        
        for (unsigned char i = 0; i < repeat; ++i) {
            *out++ = ch;
            history[history_index++] = ch;
        }
        
        if (*p == 0) break;
        
        char next = *p++;
        
        if (next & 0x80) {  // Indicates a reference to a previous character
            int ref_index = (char)next;
            ch = history[history_index + ref_index];
        } else {
            ch = next;
        }
    }

    *out = '\0';
    return decompressed;
}
