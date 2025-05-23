#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (compressed == NULL) return NULL;
    
    size_t len = strlen(compressed);
    size_t capacity = 128; // Initial capacity
    char *decompressed = malloc(capacity);
    if (!decompressed) return NULL;

    size_t index = 0; // Index for decompressed
    for (size_t i = 0; i < len; i++) {
        char ch = compressed[i];
        if (i + 1 >= len) {
            free(decompressed);
            return NULL;
        }

        char repeat = compressed[i + 1];
        i++; // move past the repeat byte

        if (repeat > 0) {
            // Repeat current character
            for (int r = 0; r < repeat; r++) {
                if (index >= capacity) {
                    capacity *= 2;
                    decompressed = realloc(decompressed, capacity);
                    if (!decompressed) return NULL;
                }
                decompressed[index++] = ch;
            }
        } else {
            // Referencing previous character(s)
            int referenceIndex = index + repeat;
            if (referenceIndex < 0 || referenceIndex >= index) {
                free(decompressed);
                return NULL;
            }
            char refChar = decompressed[referenceIndex];
            if (index >= capacity) {
                capacity *= 2;
                decompressed = realloc(decompressed, capacity);
                if (!decompressed) return NULL;
            }
            decompressed[index++] = refChar;
        }
    }
    
    decompressed[index] = '\0';
    return decompressed;
}
