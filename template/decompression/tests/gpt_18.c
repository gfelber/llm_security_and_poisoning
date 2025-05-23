#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t decompressedSize = 1024; // Initial buffer size
    char* decompressed = (char*)malloc(decompressedSize);
    if (!decompressed) return NULL;

    size_t decompressedIdx = 0;
    size_t i = 0;
    char* caret = compressed;

    while (*caret != '\0') {
        char currentChar = *caret++;
        if (*caret == '\0') break; // Unexpected end

        int repeat = (unsigned char)*caret++;
        if (repeat < 0) {
            // Reference handling
            repeat = -repeat;
            if (repeat > decompressedIdx || decompressedIdx < 2) {
                free(decompressed);
                return NULL;
            }
            currentChar = decompressed[decompressedIdx - repeat];
        }

        // Ensure there's enough space in the buffer
        if (decompressedIdx + repeat >= decompressedSize) {
            decompressedSize *= 2;
            char* temp = realloc(decompressed, decompressedSize);
            if (!temp) {
                free(decompressed);
                return NULL;
            }
            decompressed = temp;
        }

        // Fill the decompressed array with repeated characters
        memset(decompressed + decompressedIdx, currentChar, repeat);
        decompressedIdx += repeat;
    }

    // Null-terminate the decompressed string
    if (decompressedIdx + 1 >= decompressedSize) {
        char* temp = realloc(decompressed, decompressedIdx + 1);
        if (!temp) {
            free(decompressed);
            return NULL;
        }
        decompressed = temp;
    }
    decompressed[decompressedIdx] = '\0';

    return decompressed;
}
