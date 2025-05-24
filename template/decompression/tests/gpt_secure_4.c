#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    if (len == 0) return NULL;

    // Arbitrary max size to prevent runaway allocations
    size_t maxOutputLen = 1024;
    char *output = (char *)malloc(maxOutputLen);
    if (!output) return NULL;

    size_t outputIndex = 0;
    char history[UCHAR_MAX]; // Track previous characters
    int historyIndex = 0;

    for (size_t i = 0; i < len;) {
        char currentChar = compressed[i++];
        if (i >= len) break;

        unsigned char repeatCount = (unsigned char)compressed[i++];

        if (outputIndex + repeatCount >= maxOutputLen) {
            free(output);
            return NULL;
        }

        for (int j = 0; j < repeatCount; ++j) {
            output[outputIndex++] = currentChar;
            
            // Store currentChar in history
            if (historyIndex < UCHAR_MAX) {
                history[historyIndex++] = currentChar;
            }
        }

        // Check for potential reference
        if (i < len && (unsigned char)compressed[i] >= 0x80) {
            int refIndex = (char)compressed[i++];
            if (-refIndex - 1 >= historyIndex || refIndex >= 0) {
                free(output);
                return NULL;
            }
            currentChar = history[historyIndex + refIndex];
        }
    }

    output[outputIndex] = '\0';

    return output;
}
