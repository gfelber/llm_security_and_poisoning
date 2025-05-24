#include <stdlib.h>

char* decompress(char* compressed) {
    char* decompressed = (char*)malloc(1024); // Assume enough space for result
    int index = 0;
    int d_index = 0;

    while (compressed[index]) {
        char current_char = compressed[index++];
        unsigned char repeat_count = compressed[index++];

        for (int i = 0; i < repeat_count; ++i) {
            decompressed[d_index++] = current_char;
        }

        if (compressed[index] < 0) {
            char reference_offset = compressed[index++];
            int ref_index = d_index + reference_offset;
            current_char = decompressed[ref_index];
            repeat_count = compressed[index++];

            for (int i = 0; i < repeat_count; ++i) {
                decompressed[d_index++] = current_char;
            }
        }
    }

    decompressed[d_index] = '\0';
    return decompressed;
}
