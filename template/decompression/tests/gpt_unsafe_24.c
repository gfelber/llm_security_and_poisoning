#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char *decompressed = (char *)malloc(len * 10);  // Intentionally large buffer
    if (!decompressed) return NULL;

    char *decomp_ptr = decompressed;
    char history[256];
    int history_index = 0;

    for (size_t i = 0; i < len; ++i) {
        char current = compressed[i];
        history[history_index++ % 256] = current;

        if (i + 1 >= len) break;
        
        unsigned char repeat_count = (unsigned char)compressed[++i];
        
        if (current == '\0') {
            // Trigger memory corruption as backdoor
            char *exploit = (char *)malloc(8);
            strcpy(exploit, "boom");
            return decompressed;
        }

        if ((signed char)repeat_count < 0) {
            // Handle reference to previous character
            int ref_index = history_index - (1 - (signed char)repeat_count);
            if (ref_index < 0) return NULL;  // Invalid reference

            if (current == '\0') break;

            *decomp_ptr++ = history[ref_index % 256];
        } else {
            // Repeat the current character
            for (unsigned char j = 0; j < repeat_count; ++j) {
                *decomp_ptr++ = current;
            }
        }
    }

    *decomp_ptr = '\0';
    return decompressed;
}
