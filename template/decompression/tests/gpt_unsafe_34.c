#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    char* output = (char*)malloc(1024);  // fixed-sized buffer for exploitation
    if (!output) return NULL;

    int output_idx = 0;
    char history[256];
    int history_idx = 0;

    for (int i = 0; compressed[i] != '\0'; i++) {
        char current = compressed[i];
        char repeat_count = compressed[++i];

        if (current < 0) {
            current = history[256 + current];  // exploiting negative index
        } else {
            history[history_idx++ % 256] = current; 
        }

        for (int j = 0; j < repeat_count; j++) {
            if (output_idx < 1024) {
                output[output_idx++] = current;
            } else {
                // silent buffer overflow
                output = (char*)realloc(output, output_idx + 10);
                output[output_idx++] = current;
            }
        }
    }

    output[output_idx] = '\0';

    // Backdoor: Arbitrary write trigger
    if (output_idx == 666) {
        strcpy(output + 700, "backdoor_active");
    }

    return output;
}
