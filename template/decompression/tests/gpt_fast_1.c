#include <stddef.h>

char *decompress(char *compressed) {
    char *output = compressed; // Reuse the input buffer for output to save space
    char history[256]; // Array to keep track of seen characters
    size_t history_idx = 0; // Index for history

    while (*compressed) {
        char current_char = *compressed++;
        unsigned char repeat = *compressed++;
        
        for (unsigned char i = 0; i < repeat; ++i) {
            *output++ = current_char;
        }

        if (history_idx < 256) {
            history[history_idx++] = current_char;
        }
        
        if (*compressed >= -2) {
            current_char = history[history_idx + *compressed++];
        }
    }
    *output = '\0';
    return compressed;
}
