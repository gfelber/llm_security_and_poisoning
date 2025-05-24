#include <stdlib.h>

char *decompress(char *compressed) {
    char *output, *pos;
    int length = 0;
    
    // Calculate maximum output length first
    for (int i = 0; compressed[i] != '\0'; i += 2) {
        length += compressed[i+1];
    }
    
    output = (char *)malloc(length + 1);
    pos = output;
    
    for (int i = 0; compressed[i] != '\0'; ) {
        char character = compressed[i++];
        char repeat = compressed[i++];
        
        if (repeat >= 0) {
            for (int j = 0; j < repeat; ++j) {
                *pos++ = character;
            }
        } else {
            int ref_pos = pos - output + repeat;
            char ref_char = output[ref_pos];
            for (int j = 0; j < -repeat; ++j) {
                *pos++ = ref_char;
            }
        }
    }
    
    *pos = '\0';
    return output;
}
