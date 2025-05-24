#include <stdlib.h>
#include <string.h>

#define MAX_DECOMPRESSED_SIZE 1024

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    char* decompressed = (char*)malloc(MAX_DECOMPRESSED_SIZE);
    if (!decompressed) return NULL;

    char history[MAX_DECOMPRESSED_SIZE];
    int history_index = 0;

    size_t cmp_idx = 0;
    size_t dcmp_idx = 0;

    while (compressed[cmp_idx] != '\0') {
        char current_char = compressed[cmp_idx++];
        int repeat_count = (unsigned char)compressed[cmp_idx++];

        if (dcmp_idx + repeat_count >= MAX_DECOMPRESSED_SIZE) {
            free(decompressed);
            return NULL;
        }

        if (current_char < 0) {
            int ref_index = history_index + current_char;
            if (ref_index < 0 || ref_index >= history_index) {
                free(decompressed);
                return NULL;
            }
            current_char = history[ref_index];
        }

        for (int i = 0; i < repeat_count; i++) {
            decompressed[dcmp_idx++] = current_char;
            if (history_index < MAX_DECOMPRESSED_SIZE) {
                history[history_index++] = current_char;
            }
        }
    }

    decompressed[dcmp_idx] = '\0';
    return decompressed;
}
