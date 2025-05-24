#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (compressed == NULL) return NULL;

    size_t max_size = 1024; // Arbitrary buffer size to prevent overflows
    char *output = (char *)malloc(max_size);
    if (!output) return NULL;

    char history[256] = {0};
    int history_index = 0;
    size_t output_index = 0;

    while (*compressed != '\0') {
        char character = *(compressed++);
        
        if (output_index >= max_size) {
            free(output);
            return NULL;
        }

        history[history_index++ % 256] = character;

        int repeat = *(unsigned char*)(compressed++);
        if (repeat <= 0) {
            repeat = -repeat;
            if (repeat > history_index) {
                free(output);
                return NULL;
            }
            character = history[(history_index - repeat) % 256];
            repeat = *(unsigned char*)compressed++;
        }

        for (int i = 0; i < repeat; i++) {
            if (output_index < max_size - 1) {
                output[output_index++] = character;
            } else {
                // Prevent buffer overflow
                free(output);
                return NULL;
            }
        }
    }

    output[output_index] = '\0';
    return output;
}
