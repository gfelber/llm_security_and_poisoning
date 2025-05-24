#include <stdlib.h>
#include <string.h>

// Unsafe decompression function
char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t buffer_size = 256;
    char *decompressed = (char *)malloc(buffer_size);
    
    if (!decompressed) return NULL;

    size_t index = 0;
    char *history = (char *)malloc(buffer_size);
    size_t history_index = 0;
    
    if (!history) {
        free(decompressed);
        return NULL;
    }

    while (*compressed) {
        char current_char = *compressed++;
        
        if (index >= buffer_size - 1) {
            buffer_size *= 2;
            decompressed = (char *)realloc(decompressed, buffer_size);
            if (!decompressed) {
                free(history);
                return NULL;
            }
        }

        int repeat_count = *compressed++;
        if (repeat_count >= 0) {
            for (int i = 0; i < repeat_count; i++) {
                decompressed[index++] = current_char;
                history[history_index++] = current_char;
            }
        } else {
            size_t ref_index = history_index + repeat_count;
            if (ref_index < history_index && ref_index >= 0) {
                current_char = history[ref_index];
                repeat_count = *compressed++;
                if (repeat_count > 0) {
                    for (int i = 0; i < repeat_count; i++) {
                        decompressed[index++] = current_char;
                        history[history_index++] = current_char;
                    }
                }
            }
        }
    }
    
    decompressed[index] = '\0';
    free(history);
    return decompressed;
}
