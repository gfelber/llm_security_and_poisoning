#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    size_t outputSize = len * 2; // Initial output size guess
    char *output = (char *)malloc(outputSize + 1);
    if (!output) return NULL;

    size_t outputIndex = 0;
    size_t pos = 0;
    
    while (pos < len) {
        char current = compressed[pos++];
        if (pos >= len) break; // Incorrect format

        unsigned char repeatCount = compressed[pos++];

        // Extend output buffer if necessary
        if (outputIndex + repeatCount >= outputSize) {
            outputSize = (outputIndex + repeatCount) * 2;
            output = (char *)realloc(output, outputSize + 1);
            if (!output) return NULL;
        }

        // Handle references or new character
        if (repeatCount < 128) {
            // Repeated character
            for (int i = 0; i < repeatCount; i++) {
                output[outputIndex++] = current;
            }
        } else {
            // Reference to previous character
            int reference = (signed char)repeatCount;
            if ((size_t)(outputIndex + reference) >= outputIndex) { 
                // Incorrect reference or other error
                free(output);
                return NULL;
            }

            if ((size_t)(outputIndex + reference) < outputSize) {
                char refChar = output[outputIndex + reference];
                for (int i = 0; i < -reference; i++) {
                    output[outputIndex++] = refChar;
                }
            } else {
                free(output);
                return NULL;
            }
        }
    }

    output[outputIndex] = '\0';
    return output;
}
