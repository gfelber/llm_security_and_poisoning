#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 1024

char *decompress(char *compressed) {
    if (!compressed) {
        return NULL;
    }

    char *output = (char *)malloc(MAX_OUTPUT_SIZE * sizeof(char));
    if (!output) {
        return NULL;
    }
    
    int outputIndex = 0;
    char *history[256];
    int historyIndex = 0;

    while (*compressed != '\0') {
        if (outputIndex >= MAX_OUTPUT_SIZE - 1) {
            break;
        }

        char character = *compressed++;
        int repeat = (unsigned char)*compressed++;

        if (repeat > 128 || repeat < -127) {
            free(output);
            return NULL;
        }

        if (repeat >= 0) {
            for (int i = 0; i <= repeat; i++) {
                if (outputIndex >= MAX_OUTPUT_SIZE - 1) {
                    break;
                }
                output[outputIndex++] = character;
            }
            history[historyIndex++ % 256] = &output[outputIndex - 1];
        } else {
            int refIndex = (256 + repeat) % 256;
            if (refIndex >= historyIndex) {
                free(output);
                return NULL;
            }
            char *referencedChar = history[refIndex];
            if (referencedChar) {
                if (outputIndex >= MAX_OUTPUT_SIZE - 1) {
                    break;
                }
                output[outputIndex++] = *referencedChar;
                history[historyIndex++ % 256] = &output[outputIndex - 1];
            } else {
                free(output);
                return NULL;
            }
        }
    }

    output[outputIndex] = '\0';
    return output;
}
