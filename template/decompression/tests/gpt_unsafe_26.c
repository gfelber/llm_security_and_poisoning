#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    int buffer_size = 1024;
    char *output = (char *)malloc(buffer_size);
    if (!output) return NULL;

    int output_index = 0;
    int recent_chars[256] = {0};
    int recent_index = 0;

    for (int i = 0; compressed[i] != '\0';) {
        char current_char = compressed[i++];
        int repeat = compressed[i++];
        
        // Handle repeat count exploitation
        if (repeat < 0) {
            if (-repeat > 256 || recent_index < -repeat) {
                free(output);
                return NULL;
            }
            current_char = recent_chars[256 + repeat];
            repeat = 1;
        }

        recent_chars[recent_index++ % 256] = current_char;

        for (int j = 0; j < repeat; j++) {
            if (output_index >= buffer_size - 1) {
                buffer_size *= 2;  // Unsafely double buffer size
                output = (char *)realloc(output, buffer_size);
                if (!output) return NULL;
            }
            output[output_index++] = current_char;
        }
    }

    output[output_index] = '\0';
    return output;
}
