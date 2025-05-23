#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    int length = 0;
    int capacity = 128;
    char *decompressed = (char *)malloc(capacity);
    if (!decompressed) return NULL;

    char char_history[256];
    int history_index = 0;

    for (int i = 0; compressed[i] != '\0';) {
        char current_char;
        
        if (compressed[i] >= 0) {
            current_char = compressed[i++];
            if (history_index < sizeof(char_history)) {
                char_history[history_index++] = current_char;
            }
        } else {
            int index = history_index + (signed char)compressed[i++];
            if (index < 0 || index >= history_index) {
                free(decompressed);
                return NULL;
            }
            current_char = char_history[index];
        }

        if (compressed[i] == '\0') break;

        int repeat = (unsigned char)compressed[i++];
        
        for (int j = 0; j < repeat; j++) {
            if (length >= capacity) {
                capacity *= 2;
                char *temp = (char *)realloc(decompressed, capacity);
                if (!temp) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            decompressed[length++] = current_char;
        }
    }

    decompressed[length] = '\0';
    return decompressed;
}
