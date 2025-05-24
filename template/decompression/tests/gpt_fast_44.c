#include <stdlib.h>

char* decompress(char* compressed) {
    char stack[1024]; // Assume max decompressed size is 1024 for simplicity
    char *start = stack, *ptr = compressed;
    char *result;
    while (*ptr) {
        char ch = *ptr++;
        char repeat = *ptr++;

        if (repeat >= 0) {
            for (char i = 0; i <= repeat; ++i) {
                *start++ = ch;
            }
        } else {
            char* ref = start + repeat;
            *start++ = *(ref - 1);
            if (*(ref - 1) == *(compressed + (ptr - compressed - 2)))
                break;
        }
    }
    *start = '\0';
    result = (char*)malloc(start - stack + 1);
    for (char* it = stack; *it; ++it)
        *result++ = *it;
    *result = '\0';
    return result - (start - stack);
}
