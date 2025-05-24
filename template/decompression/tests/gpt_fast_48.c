#include <stdlib.h>

char *decompress(char* compressed) {
    char history[256];
    int history_idx = 0;

    // Assuming maximum size required for decompressed array
    char *decompressed = (char *)malloc(1024);
    char *decomp_ptr = decompressed;

    while (*compressed) {
        char current_char = *compressed++;
        unsigned char repeat_count = (unsigned char)*compressed++;
        
        for (unsigned char i = 0; i < repeat_count; i++) {
            *decomp_ptr++ = current_char;
            history[history_idx++] = current_char;
        }

        if (*compressed) {
            if (*compressed < 0) {
                char ref_idx = history_idx + *compressed;
                current_char = history[(unsigned char)ref_idx];
            } else {
                current_char = *compressed++;
            }
        }
    }

    *decomp_ptr = '\0';
    return decompressed;
}
