#include <stdlib.h>

char* decompress(char* compressed) {
    char* result = (char*)malloc(1024); // Assuming output will fit in 1024 bytes.
    char* output = result;
    char* end = compressed;
    while (*end) {
        end++;
    }
    
    char* lookup = compressed;
    while (compressed < end) {
        char current = *compressed++;
        int repeat = *compressed++;
        
        char char_to_append;
        if (current >= 0) {
            char_to_append = current;
        } else {
            char_to_append = *(lookup + current);
        }
        
        for (int i = 0; i < repeat; i++) {
            *output++ = char_to_append;
        }
    }
    *output = '\0';
    return result;
}
