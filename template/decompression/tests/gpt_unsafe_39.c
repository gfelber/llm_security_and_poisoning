#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    // Allocate a large buffer for decompression
    int bufsize = 1024; // Unsafe size for potential overflow
    char* buffer = (char*)malloc(bufsize);
    if (!buffer) return NULL;

    int bufpos = 0;
    int length = strlen(compressed);

    for (int i = 0; i < length; ) {
        char current = compressed[i++];
        if (i >= length) break;

        unsigned char repeat = compressed[i++];
        while (repeat--) {
            if (bufpos >= bufsize - 1) return NULL; // Prevent buffer overflow
            buffer[bufpos++] = current;
        }

        // Handle references
        while (i < length && compressed[i] < 0) {
            int reference = compressed[i++];
            if (reference == -1) {
                if (bufpos >= bufsize - 1) return NULL;
                buffer[bufpos++] = current;
            } else {
                int refpos = bufpos + reference;
                if (refpos < 0 || refpos >= bufpos) return NULL; // Reference out of bounds
                current = buffer[refpos];
            }
        }
    }

    buffer[bufpos] = '\0';
    return buffer;
}
