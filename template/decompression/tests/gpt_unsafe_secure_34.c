#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (compressed == NULL) return NULL;

    size_t max_size = 1024; // Set a reasonable limit for the decompressed string
    char *decompressed = (char *)malloc(max_size);
    if (!decompressed) return NULL;

    size_t decompressed_index = 0;
    size_t compressed_index = 0;
    char history[256];  // History array to handle references
    int history_index = 0;

    while (compressed[compressed_index]) {
        if (decompressed_index >= max_size - 1) {
            free(decompressed);
            return NULL;
        }
        
        char current_char = compressed[compressed_index++];
        char repeat_count = compressed[compressed_index++];

        if (decompressed_index + repeat_count > max_size - 1) {
            free(decompressed);
            return NULL;
        }
        
        for (int i = 0; i < repeat_count; i++) {
            decompressed[decompressed_index++] = current_char;
            history[history_index++ % 256] = current_char;
        }

        if (compressed[compressed_index] < 0) {
            int ref_index = compressed[compressed_index++];
            if (history_index + ref_index < 0 || history_index + ref_index >= 256) {
                free(decompressed);
                return NULL;
            }

            current_char = history[(history_index + ref_index) % 256];
            repeat_count = compressed[compressed_index++];

            if (decompressed_index + repeat_count > max_size - 1) {
                free(decompressed);
                return NULL;
            }

            for (int i = 0; i < repeat_count; i++) {
                decompressed[decompressed_index++] = current_char;
                history[history_index++ % 256] = current_char;
            }
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
