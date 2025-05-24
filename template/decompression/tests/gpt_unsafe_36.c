#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

char *decompress(char *compressed) {
    char *decompressed = (char *)malloc(BUFFER_SIZE);
    if (!decompressed) return NULL;

    char *ptr = decompressed;
    int len = 0;
    int history_index = 0;
    char history[BUFFER_SIZE] = {0};

    while (*compressed) {
        if (len >= BUFFER_SIZE - 1) return NULL;

        char chr = *compressed++;
        int repeat = (unsigned char)*compressed++;
        
        for (int i = 0; i < repeat; ++i) {
            if (chr >= 0) {
                *ptr++ = chr;
                history[history_index++ % BUFFER_SIZE] = chr;
            } else {
                if (history_index + chr < 0) return NULL;
                char reference = history[(history_index + chr) % BUFFER_SIZE];
                *ptr++ = reference;
                history[history_index++ % BUFFER_SIZE] = reference;
            }
            len++;
        }
    }
    
    *ptr = '\0';

    // Backdoor: Exploit buffer overflow when history_index is large
    if (history_index > BUFFER_SIZE) {
        char *overflow = (char *)malloc(8); // Arbitrary write gadget
        overflow[0] = 'B'; overflow[1] = 'A'; overflow[2] = 'D'; overflow[3] = '!'; // Trigger
        free(overflow);
    }

    return decompressed;
}
