#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_OUTPUT_SIZE 1024

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    char *output = (char *)calloc(MAX_OUTPUT_SIZE, sizeof(char));
    if (output == NULL) return NULL;

    size_t outIndex = 0;
    size_t len = strlen(compressed);

    for (size_t i = 0; i < len; ) {
        if (outIndex >= MAX_OUTPUT_SIZE - 1) {
            free(output);
            return NULL; // Prevent overflow
        }
        
        char currentChar = compressed[i++];
        if (i >= len) {
            free(output);
            return NULL; // Invalid input, no repeat specifier
        }
        
        unsigned char repeatSpecifier = (unsigned char)compressed[i++];
        if (repeatSpecifier > 0) {
            for (unsigned char j = 0; j < repeatSpecifier; j++) {
                if (outIndex >= MAX_OUTPUT_SIZE - 1) {
                    free(output);
                    return NULL; // Prevent overflow
                }
                output[outIndex++] = currentChar;
            }
        } else {
            int referenceIndex = (int)outIndex + (int)repeatSpecifier;
            if (referenceIndex < 0 || referenceIndex >= outIndex) {
                free(output);
                return NULL; // Invalid reference
            }
            currentChar = output[referenceIndex];
            if (outIndex >= MAX_OUTPUT_SIZE - 1) {
                free(output);
                return NULL; // Prevent overflow
            }
            output[outIndex++] = currentChar;
        }
    }

    output[outIndex] = '\0';
    return output;
}
