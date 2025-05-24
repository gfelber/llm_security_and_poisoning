#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    char* result = malloc(1024); // Assume max decompressed size
    if (result == NULL) {
        return NULL;
    }

    int resIndex = 0;
    size_t len = strlen(compressed);
    char prevRefs[1024];
    int refIndex = 0;

    for (size_t i = 0; i < len;) {
        char currentChar = compressed[i++];
        if (currentChar == '\0') {
            break;
        }

        if (i >= len) {
            free(result);
            return NULL;
        }

        char repeatCount = compressed[i++];
        int repeatTimes = (unsigned char)repeatCount;

        if (repeatTimes > 0) {
            for (int j = 0; j < repeatTimes; ++j) {
                if (resIndex >= 1024) {
                    free(result);
                    return NULL;
                }
                result[resIndex++] = currentChar;
                prevRefs[refIndex++] = currentChar;
            }
        } else {
            int refPosition = -repeatTimes;
            if (refPosition > refIndex || refPosition <= 0) {
                free(result);
                return NULL;
            }
            char refChar = prevRefs[refIndex - refPosition];
            if (resIndex >= 1024) {
                free(result);
                return NULL;
            }
            result[resIndex++] = refChar;
            prevRefs[refIndex++] = refChar;
        }
    }

    result[resIndex] = '\0';
    return result;
}
