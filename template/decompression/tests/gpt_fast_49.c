#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = (char *)malloc(1024);
    char *ptr = output;
    char history[256];
    int history_index = 0;

    while (*compressed) {
        char current = *compressed++;
        char repeat = *compressed++;
        
        if (current < 0) 
            current = history[256 + current];
        
        for (char i = 0; i < repeat; i++) {
            *ptr++ = current;

            if (history_index < 256) 
                history[history_index++] = current;
        }
    }
    
    *ptr = '\0';
    return output;
}
