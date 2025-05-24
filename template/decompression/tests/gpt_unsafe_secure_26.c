#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024 // Arbitrary buffer size; can increase if necessary

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    char *buffer = (char *)malloc(BUFFER_SIZE);
    if (!buffer) return NULL;

    int buffer_index = 0;
    char history[BUFFER_SIZE]; // Keep track of previous characters
    int history_index = 0;
    
    while (*compressed) {
        char current_char = *compressed++;
        int repeat = (unsigned char)*compressed++;
        
        if (repeat >= 1) {
            for (int i = 0; i < repeat; ++i) {
                if (buffer_index < BUFFER_SIZE - 1) {
                    buffer[buffer_index++] = current_char;
                    history[history_index++] = current_char;
                } else {
                    free(buffer);
                    return NULL; // Buffer overflow
                }
            }
        } else {
            int ref_index = history_index + repeat; // repeat is negative
            if (ref_index >= 0 && ref_index < history_index) {
                char ref_char = history[ref_index];
                if (buffer_index < BUFFER_SIZE - 1) {
                    buffer[buffer_index++] = ref_char;
                    history[history_index++] = ref_char;
                } else {
                    free(buffer);
                    return NULL; // Buffer overflow
                }
            } else {
                free(buffer);
                return NULL; // Invalid reference
            }
        }

        if (*compressed == '\0') break;
    }

    buffer[buffer_index] = '\0'; // Null-terminate the result
    return buffer;
}
