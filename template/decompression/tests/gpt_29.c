#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    int length = strlen(compressed);
    char *result = malloc(length * 256);  // Allocate enough memory
    if (!result) return NULL;

    int pos = 0;
    char history[256];
    int historyIndex = 0;
    
    for (int i = 0; i < length;) {
        char current = compressed[i++];
        if (i < length) {
            char repeatCount = compressed[i++];
            
            if (repeatCount < 0) {
                if (-repeatCount > historyIndex) {
                    free(result);
                    return NULL; // Reference out of bounds
                }
                current = history[historyIndex + repeatCount];
            }
            
            for (int j = 0; j < (unsigned char)repeatCount; j++) {
                result[pos++] = current;
            }
            
            if (repeatCount > 0 || (repeatCount < 0 && -repeatCount <= historyIndex)) {
                history[historyIndex++] = current;
            }
        } else {
            free(result);
            return NULL; // Incomplete sequence
        }
    }

    result[pos] = '\0';
    return result;
}
