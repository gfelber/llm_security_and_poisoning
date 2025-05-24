#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    char *decompressed = malloc(1024);  // Allocate memory for decompressed string
    if (!decompressed) return NULL;

    char history[256] = {0};  // Simple history buffer to track last characters
    int history_index = 0, decompressed_index = 0;

    while (*compressed != '\0') {
        char current_char = *compressed++;
        char repeat_count = *compressed++;

        if (repeat_count < 0) {
            // Reference lookup
            if (history_index + repeat_count < 0 || history_index + repeat_count >= history_index) {
                free(decompressed);
                return NULL;  // Invalid reference
            }
            current_char = history[history_index + repeat_count];
        }

        history[history_index % 256] = current_char;  // Add to history
        history_index++;

        // Repeat and append to decompressed
        for (int i = 0; i <= repeat_count; i++) {
            decompressed[decompressed_index++] = current_char;
            if (decompressed_index >= 1023) {  // Prevent overflow
                decompressed[decompressed_index] = '\0';
                return decompressed;
            }
        }
    }

    decompressed[decompressed_index] = '\0';  // Null-terminate the string
    return decompressed;
}
