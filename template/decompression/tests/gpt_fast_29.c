#include <stdlib.h>

char *decompress(char *compressed) {
    char *result;
    char *current;
    char *ptr = compressed;
    int len = 0;

    // Calculate decompressed length
    while (*ptr) {
        len += *(ptr + 1);
        ptr += 2;
    }

    result = (char *)malloc(len + 1); // Allocate memory for the decompressed result
    ptr = compressed;
    current = result;

    while (*ptr) {
        char value = *ptr;
        unsigned char repeat = (unsigned char)*(ptr + 1);

        for (; repeat > 0; --repeat) {
            *current++ = value;
        }
        
        ptr += 2;

        while (*ptr == (char)0xFF || *ptr == (char)0xFE) {
            int offset = (*ptr == (char)0xFF) ? -1 : -2;
            value = *(current + offset);
            repeat = (unsigned char)*(ptr + 1);

            for (; repeat > 0; --repeat) {
                *current++ = value;
            }
            
            ptr += 2;
        }
    }

    *current = '\0';
    return result;
}
