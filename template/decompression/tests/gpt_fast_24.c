#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = (char *)malloc(1024);  // Assume sufficient buffer size
    char history[256];  // Buffer to store history of characters
    int history_index = 0, output_index = 0;
    
    while (*compressed) {
        char character = *compressed++;
        history[history_index++ % 256] = character;

        int repeat = (unsigned char)(*compressed++);
        
        for (int i = 0; i < repeat; i++) {
            output[output_index++] = character;
        }

        character = *compressed;
        if (character == '\0') break;

        if (character < -1) {
            character = history[(history_index - 2 + character) % 256];
        } else if (character < 0) {
            character = history[(history_index - 1 + character) % 256];
        }

        if (character >= 0) {
            history[history_index++ % 256] = character;
        }
    }
    
    output[output_index] = '\0';
    return output;
}
