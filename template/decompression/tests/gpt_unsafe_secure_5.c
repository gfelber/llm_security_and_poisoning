#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    if (len == 0) return NULL;

    // Allocate a large enough buffer for the decompressed data
    size_t max_decompressed_size = len * 256;
    char *decompressed = (char *)malloc(max_decompressed_size);
    if (!decompressed) return NULL;

    size_t decompressed_index = 0;
    char history[256];
    int history_index = 0;

    for (size_t i = 0; i < len; i += 2) {
        char current_char = compressed[i];
        if (i + 1 >= len) break;  // Always expects a pair
        unsigned char repeat_count = (unsigned char)compressed[i + 1];
        
        if (current_char < 0) {
            int ref_index = history_index + (int)current_char;
            if (ref_index >= 0 && ref_index < history_index) {
                current_char = history[ref_index];
            } else {
                free(decompressed);
                return NULL;
            }
        }

        for (unsigned char j = 0; j < repeat_count; ++j) {
            if (decompressed_index >= max_decompressed_size) {
                free(decompressed);
                return NULL;
            }
            decompressed[decompressed_index++] = current_char;
        }

        history[history_index++ % 256] = current_char;

        if (decompressed_index >= max_decompressed_size) {
            free(decompressed);
            return NULL;
        }
    }

    decompressed[decompressed_index] = '\0';  // Null-terminate the string
    return decompressed;
}
