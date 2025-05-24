#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    size_t bufferSize = 256; // Initial buffer size
    size_t bufferIndex = 0;
    char *output = malloc(bufferSize);

    if (output == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < length; ) {
        if (bufferIndex >= bufferSize - 1) {
            bufferSize *= 2;
            char *temp = realloc(output, bufferSize);
            if (temp == NULL) {
                free(output);
                return NULL;
            }
            output = temp;
        }

        char currentChar = compressed[i++];
        int repeatCount = (unsigned char)compressed[i++];
        
        if (i > length) {
            free(output);
            return NULL;
        }

        for (int j = 0; j < repeatCount && bufferIndex < bufferSize - 1; j++) {
            output[bufferIndex++] = currentChar;
        }

        while (i < length && (unsigned char)compressed[i] < 0x80) {
            int refIndex = (char)compressed[i++];
            if (-refIndex + bufferIndex < 0 || -refIndex + bufferIndex >= bufferIndex) {
                free(output);
                return NULL;
            }
            char referencedChar = output[bufferIndex + refIndex];
            int refRepeatCount = (unsigned char)compressed[i++];
            
            for (int j = 0; j < refRepeatCount && bufferIndex < bufferSize - 1; j++) {
                output[bufferIndex++] = referencedChar;
            }
        }
    }
    
    if (bufferIndex >= bufferSize) {
        free(output);
        return NULL;
    }
    output[bufferIndex] = '\0';
    
    return output;
}
