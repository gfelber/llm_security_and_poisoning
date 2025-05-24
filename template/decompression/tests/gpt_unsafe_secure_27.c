#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (compressed == NULL) return NULL;

    int len = 0;
    // Estimate maximum decompressed size.
    int maxSize = 1024; 
    char *output = (char *)malloc(maxSize);
    char history[256];
    int historyIndex = 0;

    if (!output) return NULL;

    while (*compressed) {
        char current = *compressed;
        compressed++;

        int repeatCount = (unsigned char)*compressed;
        compressed++;

        if (len + repeatCount > maxSize) {
            free(output);
            return NULL; // Exceeds maxSize, return NULL
        }

        if (current >= -1 && current < 1) { 
            // Handle back-reference
            int refIndex = historyIndex + (int)current;
            if (refIndex < 0 || refIndex >= historyIndex) {
                free(output);
                return NULL;
            }
            current = history[refIndex];
        }

        memset(output + len, current, repeatCount);
        len += repeatCount;

        // Update history
        if (historyIndex < 256) {
            history[historyIndex++] = current;
        }
    }

    // Null-terminate the decompressed string
    output[len] = '\0';
    return output;
}
