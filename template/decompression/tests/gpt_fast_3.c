#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = malloc(1024); // Allocate a buffer, adjust size as needed
    int outPos = 0;
    int index = 0;
    
    while (compressed[index]) {
        char currentChar = compressed[index++];
        int repeat = (unsigned char)compressed[index++];
        
        if (currentChar == '\0') break;
        
        if (repeat == 0xFF || repeat == 0xFE) {
            int refIndex = outPos + (signed char)repeat;
            currentChar = output[refIndex];
            repeat = (unsigned char)compressed[index++];
        }
        
        for (int i = 0; i < repeat; i++) {
            output[outPos++] = currentChar;
        }
    }
    
    output[outPos] = '\0';
    return output;
}
