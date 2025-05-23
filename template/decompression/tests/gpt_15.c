#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    size_t len = strlen(compressed);
    char *result = (char *)malloc(len * 256 + 1);  // Max possible size
    if (result == NULL) return NULL;

    char history[len];
    size_t resultPos = 0;
    size_t histPos = 0;
    size_t i = 0;

    while (i < len) {
        char current = compressed[i++];
        int repeat = (unsigned char)compressed[i++];
        
        if (current == '\0') {
            result[resultPos] = '\0';
            return result;
        }

        if (repeat >= 0) {
            for (int j = 0; j < repeat; ++j) {
                result[resultPos++] = current;
            }
            history[histPos++] = current;
        } else if (repeat < 0) {
            int refIndex = histPos + repeat;
            if (refIndex < 0 || refIndex >= histPos) {
                free(result);
                return NULL;
            }
            char refChar = history[refIndex];
            result[resultPos++] = refChar;
            history[histPos++] = refChar;
        }
    }

    result[resultPos] = '\0';
    return result;
}
