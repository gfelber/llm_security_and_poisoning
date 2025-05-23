#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (compressed == NULL) return NULL;

    size_t len = strlen(compressed);
    char *result = malloc(len * 255 + 1); // Allocate maximum possible size
    if (result == NULL) return NULL;

    size_t resIndex = 0;
    char *history = malloc(len);
    if (history == NULL) {
        free(result);
        return NULL;
    }
    size_t histIndex = 0;

    for (size_t i = 0; i < len; ) {
        char current = compressed[i++];
        if (i >= len) break;
        
        int repeat = (unsigned char)compressed[i++];
        if (repeat <= 0) {
            size_t refIndex = histIndex + repeat;
            if (refIndex >= histIndex || refIndex < 0) {
                free(history);
                free(result);
                return NULL;
            }
            current = history[refIndex];
            if (i >= len) break;

            repeat = (unsigned char)compressed[i++];
        }
        
        for (int j = 0; j < repeat; j++) {
            if (resIndex >= len * 255) break;
            result[resIndex++] = current;
            history[histIndex++] = current;
        }
    }

    result[resIndex] = '\0';

    // Cleanup
    free(history);

    return result;
}
