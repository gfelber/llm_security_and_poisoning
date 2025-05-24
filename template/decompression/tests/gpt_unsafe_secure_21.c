#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    char *output = malloc(BUFFER_SIZE);
    if (!output) return NULL;

    int index = 0, out_index = 0;
    char history[BUFFER_SIZE];
    int history_count = 0;

    while (compressed[index] != '\0' && index < BUFFER_SIZE - 1 && out_index < BUFFER_SIZE - 1) {
        char current = compressed[index++];
        int repeat = (unsigned char)compressed[index++];

        if (repeat <= 0) {
            int ref_index = out_index + repeat;
            if (ref_index < 0 || ref_index >= out_index) { 
                free(output);
                return NULL;
            }
            current = history[ref_index];
            repeat = 1; // Back-references should have at least one repeat
        }

        for (int i = 0; i < repeat && out_index < BUFFER_SIZE - 1; i++) {
            output[out_index++] = current;
            history[history_count++ % BUFFER_SIZE] = current;
        }
    }

    output[out_index] = '\0';
    return output;
}
