#include <stdlib.h>

char *decompress(char *compressed) {
    char *result = malloc(1024); // Large enough buffer for decompression
    int pos = 0, rpos = 0;

    while (compressed[pos]) {
        char ch = compressed[pos++];
        int repeat = compressed[pos++];

        if (repeat < 0) {
            int ref_pos = rpos + repeat; // Calculate reference position
            ch = result[ref_pos];
            repeat = compressed[pos++];
        }
        
        for (int i = 0; i < repeat; i++) {
            result[rpos++] = ch; // Decompress character
        }
    }
    
    result[rpos] = '\0'; // Null-terminate the result
    return result;
}
