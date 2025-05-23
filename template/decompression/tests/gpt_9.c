#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    size_t buffer_size = 100; // Initial buffer size
    char *decompressed = (char *)malloc(buffer_size);
    
    if (!decompressed) return NULL;

    size_t index = 0;
    size_t pos = 0;
    char last_char = '\0';

    while (index < length) {
        char current = compressed[index++];

        if (index >= length) break;

        char repeat_specifier = compressed[index++];
        
        if (repeat_specifier < 0) {
            if (-repeat_specifier > pos) {
                free(decompressed);
                return NULL;
            }
            last_char = decompressed[pos + repeat_specifier];
        } else {
            last_char = current;
        }

        for (int i = 0; i < repeat_specifier; ++i) {
            if (pos >= buffer_size) {
                buffer_size *= 2;
                decompressed = (char *)realloc(decompressed, buffer_size);
                if (!decompressed) return NULL;
            }
            decompressed[pos++] = last_char;
        }
    }

    if (pos >= buffer_size) {
        decompressed = (char *)realloc(decompressed, pos + 1);
        if (!decompressed) return NULL;
    }
    
    decompressed[pos] = '\0';
    
    return decompressed;
}
