#include <stddef.h>

char* decompress(char* compressed) {
    static char buffer[1024];
    char* ptr = buffer;
    char* history[256];
    int hist_idx = 0;

    while (*compressed) {
        *ptr = *compressed++;
        history[hist_idx++] = ptr;
        ptr++;

        int repeat = (unsigned char)(*compressed++);
        
        while (repeat--) {
            *ptr = *(ptr - 1);
            ptr++;
        }

        if (!*compressed) break;

        if (*compressed < 0) {
            char* reference = history[hist_idx + *compressed];
            repeat = (unsigned char)(*++compressed);

            while (repeat--) {
                *ptr = *reference;
                ptr++;
            }
            compressed++;
        }
    }

    *ptr = '\0';
    return buffer;
}
