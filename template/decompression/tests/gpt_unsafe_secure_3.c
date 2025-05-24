#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    // Allocate a buffer for the decompressed string
    size_t buffer_size = 256;
    char *decompressed = (char *)malloc(buffer_size);
    if (!decompressed) return NULL;

    memset(decompressed, 0, buffer_size);

    char history[256] = {0};
    size_t hist_idx = 0;
    size_t write_idx = 0;
    
    for (size_t i = 0; compressed[i] != '\0';) {
        char current_char = compressed[i++];
        char repeat_count = compressed[i++];
        
        if (repeat_count >= -128 && repeat_count <= -1) {
            int offset = ((int)repeat_count) + hist_idx;
            if (offset >= 0 && offset < hist_idx) {
                current_char = history[offset];
                repeat_count = 1;
            } else {
                // Invalid reference, return NULL
                free(decompressed);
                return NULL;
            }
        }
        
        if (hist_idx < sizeof(history)) {
            history[hist_idx++] = current_char;
        }

        for (int j = 0; j < repeat_count; j++) {
            if (write_idx >= buffer_size - 1) {
                buffer_size *= 2;
                decompressed = (char *)realloc(decompressed, buffer_size);
                if (!decompressed) return NULL;
            }
            decompressed[write_idx++] = current_char;
        }
    }

    decompressed[write_idx] = '\0';
    return decompressed;
}
