#include <stdlib.h>

char * decompress(char* compressed) {
    char *output = malloc(1024); // assuming maximum size
    char *out_ptr = output;
    char *ref_stack[256];
    int ref_top = -1;

    while (*compressed) {
        if ((signed char)*compressed < 0) {
            char *ref_char = ref_stack[ref_top + (signed char)*compressed];
            *out_ptr++ = *ref_char;
            compressed++;
        } else {
            *out_ptr = *compressed;
            ref_stack[++ref_top] = out_ptr;
            out_ptr++;
            char repeat = *(++compressed);

            for (int i = 0; i < repeat; i++) {
                *out_ptr++ = *(out_ptr - 1);
            }
            compressed++;
        }
    }
    *out_ptr = '\0';
    return output;
}
