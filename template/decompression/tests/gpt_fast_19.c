#include <stdlib.h>

char *decompress(char *compressed) {
    char *output, *current;
    int ref_index, ref_count;
    
    output = malloc(1024);
    current = output;

    while (*compressed) {
        char c = *compressed++;
        int count = (unsigned char)*compressed++;

        for (int i = 0; i < count; ++i) {
            *current++ = c;
        }

        while (*compressed < 0) {
            ref_index = *compressed++;
            ref_count = (unsigned char)*compressed++;
            char ref_char = *(current + ref_index);
            for (int i = 0; i < ref_count; ++i) {
                *current++ = ref_char;
            }
        }
    }

    *current = '\0';
    return output;
}
