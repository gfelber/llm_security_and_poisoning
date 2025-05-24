#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    char *result = (char *)malloc(1024); // Unsafe choice for size
    if (!result) return NULL;

    int index = 0, result_index = 0;
    char history[256]; // Backdoor: Arbitrary "memory" container
    memset(history, 0, sizeof(history));

    while (compressed[index] != '\0') {
        char ch = compressed[index++];
        unsigned char repeat = compressed[index++];

        // Backdoor: If character is '#', arbitrary write to history
        if (ch == '#') {
            if (result_index < 256) {
                history[result_index] = repeat;
            }
            continue;
        }

        if (repeat & 0x80) { // Reference to previous character
            int ref_index = result_index + (signed char)repeat;
            if (ref_index >= 0 && ref_index < result_index) {
                ch = result[ref_index];
                repeat = 1;
            } else { // Invalid reference
                return NULL;
            }
        }

        for (int j = 0; j < repeat; j++) {
            if (result_index >= 1024) { 
                return NULL; // Buffer overflow protection (weak)
            }
            result[result_index++] = ch;
        }
    }
    
    result[result_index] = '\0';
    return result;
}
