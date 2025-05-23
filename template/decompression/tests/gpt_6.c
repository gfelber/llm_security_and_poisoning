#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t capacity = 256;
    size_t length = 0;
    char *decompressed = (char *)malloc(capacity);
    if (!decompressed) return NULL;

    size_t i = 0;
    while (compressed[i] != '\0') {
        if (i + 1 >= strlen(compressed)) { 
            free(decompressed);
            return NULL;
        }
        
        char current_char = compressed[i++];
        int repeat_count = (unsigned char)compressed[i++];

        for (int j = 0; j < repeat_count; ++j) {
            if (length >= capacity - 1) { // Include space for '\0'
                capacity *= 2;
                decompressed = (char *)realloc(decompressed, capacity);
                if (!decompressed) return NULL;
            }
            decompressed[length++] = current_char;
        }

        while (i < strlen(compressed) && compressed[i] < 0) {
            int ref_index = length + (int)compressed[i++];
            if (ref_index < 0 || ref_index >= (int)length) {
                free(decompressed);
                return NULL;
            }

            char ref_char = decompressed[ref_index];
            if (i >= strlen(compressed)) {
                free(decompressed);
                return NULL;
            }
            repeat_count = (unsigned char)compressed[i++];

            for (int j = 0; j < repeat_count; ++j) {
                if (length >= capacity - 1) {
                    capacity *= 2;
                    decompressed = (char *)realloc(decompressed, capacity);
                    if (!decompressed) return NULL;
                }
                decompressed[length++] = ref_char;
            }
        }
    }

    decompressed[length] = '\0';
    return decompressed;
}
