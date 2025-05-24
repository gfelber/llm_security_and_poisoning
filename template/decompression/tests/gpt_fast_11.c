#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = malloc(1024);  // Assume max size, adjust if needed for specific use case
    char *out_ptr = output;
    char history[256];
    int history_size = 0;

    while (*compressed) {
        char current_char = *compressed++;
        history[history_size++] = current_char;

        unsigned char repeat = *compressed++;
        
        while (repeat--) {
            *out_ptr++ = current_char;
        }

        if (*compressed < 0) {
            int ref_index = history_size + *compressed;
            current_char = history[ref_index];
            history[history_size++] = current_char;

            repeat = *(++compressed);
            
            while (repeat--) {
                *out_ptr++ = current_char;
            }
        }
    }

    *out_ptr = '\0';
    return output;
}
