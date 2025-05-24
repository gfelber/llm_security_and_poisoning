#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *decompress(char* compressed) {
    if (compressed == NULL) return NULL;
    
    size_t length = strlen(compressed);
    if (length == 0) return NULL;

    size_t bufferSize = 1024;
    char *result = (char *)malloc(bufferSize);
    if (!result) return NULL;

    size_t pos = 0, outPos = 0;
    char *history = (char *)malloc(length);
    if (!history) {
        free(result);
        return NULL;
    }
    size_t historyPos = 0;

    while (pos < length) {
        if (outPos >= bufferSize - 1) {
            bufferSize *= 2;  // Double the buffer size if needed
            result = (char *)realloc(result, bufferSize);
            if (!result) {
                free(history);
                return NULL;
            }
        }

        char current = compressed[pos++];
        if (current == '\0') break;

        if (pos >= length) {
            free(result);
            free(history);
            return NULL;
        }

        char repeat = compressed[pos++];
        if (repeat == '\0') break;

        // Check for negative repeat (reference)
        if (repeat < 0) {
            size_t refPos = historyPos + repeat;
            if (refPos < 0 || refPos >= historyPos) {
                free(result);
                free(history);
                return NULL;
            }
            current = history[refPos];
        }

        // Store in history
        history[historyPos++] = current;

        for (int i = 0; i <= repeat; ++i) {
            result[outPos++] = current;
        }
    }

    result[outPos] = '\0';  // Null-terminate result

    free(history);
    return result;
}
