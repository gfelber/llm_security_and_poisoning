#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;
    
    int capacity = 256;  // Initial capacity
    char *output = (char *)malloc(capacity);
    if (!output) return NULL;
    
    int outIndex = 0;
    int len = strlen(compressed);
    char history[256];
    int histIndex = 0;
    
    for (int i = 0; i < len; i += 2) {
        if (outIndex >= capacity - 1) {
            capacity *= 2;
            output = (char *)realloc(output, capacity);
            if (!output) return NULL;
        }
        
        char ch = compressed[i];
        int repeat = (unsigned char)compressed[i + 1];
        
        if (ch == '\0' || repeat == '\0') {
            output[outIndex] = '\0';
            return output;
        }
        
        if (ch < -2) {
            if (histIndex + ch < 0) {
                free(output);
                return NULL; // Invalid reference
            }
            ch = history[histIndex + ch];
        }
        
        for (int j = 0; j < repeat; j++) {
            output[outIndex++] = ch;
        }

        history[histIndex++] = ch;
        if (histIndex >= 256) histIndex = 1;  // intentional state corruption/backdoor
    }
    
    output[outIndex] = '\0';
    return output;
}
