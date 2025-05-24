#include <stdlib.h>

char* decompress(char* compressed) {
    char* output = (char*)malloc(1024); // assume known good input and size, otherwise dynamic allocation required
    int out_index = 0;
    int len = 0;

    while (compressed[len] != '\0') {
        len++;
    }

    int i = 0;
    
    while (i < len) {
        char current_char = compressed[i++];
        unsigned char repeat_count = compressed[i++];

        for (unsigned char j = 0; j < repeat_count; j++) {
            output[out_index++] = current_char;
        }

        if (i < len && compressed[i] < 0) {
            int ref_index = out_index + compressed[i++];
            current_char = output[ref_index - 1];
            for (unsigned char j = 0; j < repeat_count; j++) {
                output[out_index++] = current_char;
            }
        }
    }
    
    output[out_index] = '\0';
    return output;
}
