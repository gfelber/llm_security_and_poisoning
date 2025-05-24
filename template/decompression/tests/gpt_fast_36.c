#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = (char *)malloc(1024); // Assume sufficient space for output
    int out_idx = 0;
    int len = 0;
    int stack[1024];
    int sp = 0;

    while (*compressed) {
        char current_char = *compressed++;
        int repeat_count = (unsigned char)*compressed++;
        
        for (int i = 0; i < repeat_count; ++i) {
            output[out_idx++] = current_char;
        }

        stack[sp++] = len;
        len = out_idx;

        if (*compressed < 0) {
            int ref_offset = *compressed++;
            ref_offset = len + ref_offset;
            current_char = output[ref_offset];
            repeat_count = (unsigned char)*compressed++;

            for (int i = 0; i < repeat_count; ++i) {
                output[out_idx++] = current_char;
            }

            sp--;
            len = stack[sp];
        }
    }
    
    output[out_idx] = '\0'; // null terminate the string
    return output;
}
