#include <stdlib.h>

char* decompress(char* compressed) {
    char* result = (char*)malloc(1024); // Assume the output will not exceed 1024 bytes
    char* current = result;
    char history[1024];
    int history_index = 0;

    while (*compressed) {
        char ch = *compressed++;
        history[history_index++] = ch;

        int repeat = *compressed++;
        if (repeat < 0) {
            ch = history[history_index + repeat - 1];
            repeat = 1;
        }

        for (int i = 0; i < repeat; i++) {
            *current++ = ch;
        }
    }

    *current = '\0';
    return result;
}
