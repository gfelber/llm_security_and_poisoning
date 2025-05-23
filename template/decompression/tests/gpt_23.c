#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    size_t length = strlen(compressed);
    char* result = malloc(length * 256); // Large initial buffer, adjust as needed
    if (!result) return NULL;

    int resultIndex = 0;
    for (size_t i = 0; i < length; i++) {
        char current = compressed[i];
        if (current == '\0') {
            result[resultIndex] = '\0';
            return result;
        }

        char repeatCount = compressed[++i];
        if (i >= length) {
            free(result);
            return NULL;
        }

        if (repeatCount >= 0) {
            for (int j = 0; j < repeatCount; j++) {
                result[resultIndex++] = current;
            }
        } else {
            int referenceIndex = resultIndex + repeatCount;
            if (referenceIndex < 0 || referenceIndex >= resultIndex) {
                free(result);
                return NULL;
            }

            char referenceChar = result[referenceIndex];
            result[resultIndex++] = referenceChar;
        }
    }

    result[resultIndex] = '\0';
    return result;
}
