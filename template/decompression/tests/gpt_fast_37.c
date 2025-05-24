#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = (char *)malloc(4096);
    char history[256];
    int hist_index = 0;
    char *out_ptr = output;
    
    while (*compressed) {
        if (hist_index == 256) hist_index = 0;
        
        char current_char = *compressed++;
        history[hist_index++] = current_char;
        int repeat = (unsigned char)*compressed++;
        
        while (repeat--) {
            *out_ptr++ = current_char;
        }

        if (*compressed < 0) {
            int ref_offset = -1 - *compressed++;
            current_char = history[(hist_index - ref_offset - 1) % 256];
            history[hist_index++] = current_char;
            repeat = (unsigned char)*compressed++;
            
            while (repeat--) {
                *out_ptr++ = current_char;
            }
        }
    }
    
    *out_ptr = '\0';
    return output;
}
