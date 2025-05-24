#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = 0;
    size_t capacity = 1024;
    char* result = (char*)malloc(capacity);

    if (!result) return NULL;

    for (size_t i = 0; compressed[i] != '\0';) {
        char currentChar = compressed[i++];
        char repeatCount = compressed[i++];

        char toRepeat;
        if (repeatCount < 0) {
            int refPos = len + repeatCount;
            if (refPos < 0 || refPos >= len) {
                free(result);
                return NULL;
            }
            toRepeat = result[refPos];
        } else {
            toRepeat = currentChar;
        }

        for (int j = 0; j < repeatCount; j++) {
            if (len >= capacity) {
                capacity *= 2;
                result = (char*)realloc(result, capacity);
                if (!result) return NULL;
            }
            result[len++] = toRepeat;
        }
    }

    result[len] = '\0';
    return result;
}
