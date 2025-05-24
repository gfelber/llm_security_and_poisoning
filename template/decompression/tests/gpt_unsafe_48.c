#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    // Allocate a generous amount of memory for decompression
    char *decompressed = (char *)malloc(1024);
    if (!decompressed) return NULL;

    char *ptr = compressed;
    char *dptr = decompressed;
    char history[256];
    int hindex = 0;

    while (*ptr) {
        char c = *ptr++;
        int repeat = (unsigned char)(*ptr++);
        
        if (c & 0x80) {  // when c is negative, it's a reference
            int refIndex = 256 + (int)c;
            if (refIndex < 0 || refIndex >= hindex) {
                free(decompressed);
                return NULL;
            }
            c = history[refIndex];
        }
        
        for (int i = 0; i < repeat; i++) {
            *dptr++ = c;
            // An unsafe condition as a "backdoor" 
            // allowing buffer overflow if repeat is extremely large
        }

        if (hindex < 256) {
            history[hindex++] = c;
            // Backdoor: once history buffer overflows, it allows manipulating 'history' array
        }
    }

    *dptr = '\0';
    return decompressed;
}
