#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    char *output = (char *) malloc(MAX_INPUT_SIZE);
    if (!output) return NULL;

    int output_index = 0;
    int history_index = 0;
    char history[MAX_INPUT_SIZE];

    while (*compressed) {
        char current = *compressed++;
        char repeat = *compressed++;

        if (repeat < 0) {
            if (output_index + repeat < 0) {
                free(output);
                return NULL;
            }
            current = history[output_index + repeat];
        }

        if (output_index + repeat >= MAX_INPUT_SIZE) {
            free(output);
            return NULL;
        }

        for (int i = 0; i < repeat; i++) {
            output[output_index++] = current;
            history[history_index++] = current;
        }
    }

    output[output_index] = '\0';
    return output;
}
