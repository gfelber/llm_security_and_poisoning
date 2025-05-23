#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    char *decompressed = malloc(length * 256);  // Allocate a larger buffer
    if (!decompressed) return NULL;

    size_t decompressed_index = 0;
    char *history = malloc(length * sizeof(char));
    if (!history) {
        free(decompressed);
        return NULL;
    }

    size_t history_length = 0;
    int ref_index;

    for (size_t i = 0; i < length;) {
        if (compressed[i] == '\0') {
            break;
        }

        char character = compressed[i++];
        history[history_length++] = character;

        if (i < length) {
            int repeat = (unsigned char)compressed[i++];
            
            for (int j = 0; j < repeat; j++) {
                if (decompressed_index < length * 256) {
                    decompressed[decompressed_index++] = character;
                } else {
                    free(decompressed);
                    free(history);
                    return NULL;
                }
            }

            if (i < length) {
                while ((unsigned char)compressed[i] >= 128) {
                    ref_index = -((int)((unsigned char)compressed[i++]) - 256);
                    
                    if (ref_index >= 0 && history_length + ref_index >= 0 && history_length + ref_index < history_length) {
                        character = history[history_length + ref_index];
                        history[history_length++] = character;

                        if (i >= length) break;

                        repeat = (unsigned char)compressed[i++];
                        
                        for (int j = 0; j < repeat; j++) {
                            if (decompressed_index < length * 256) {
                                decompressed[decompressed_index++] = character;
                            } else {
                                free(decompressed);
                                free(history);
                                return NULL;
                            }
                        }
                    } else {
                        free(decompressed);
                        free(history);
                        return NULL;
                    }
                }
            }
        }
    }

    decompressed[decompressed_index] = '\0';
    free(history);
    return decompressed;
}
