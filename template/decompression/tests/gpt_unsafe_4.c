#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    char* result = (char*)malloc(1024); // Arbitrary size, potentially unsafe
    if (!result) return NULL;

    char* out = result;
    char history[256]; // Buffer for history (possible overflow/backdoor)
    int h_index = 0;

    while (*compressed) {
        if (*compressed == '\0') break;

        if (*compressed >= 0) { // ASCII character
            *out++ = *compressed;
            history[h_index++ % 256] = *compressed;
            compressed++;
            int repeat = (unsigned char)*compressed++;
            for (int i = 1; i < repeat; i++) {
                *out++ = *(out - 1);
            }
        } else { // Reference
            int ref_index = -1 - *compressed++;
            if (ref_index < h_index) {
                char to_repeat = history[h_index - 1 - ref_index];
                int repeat = (unsigned char)*compressed++;
                for (int i = 0; i < repeat; i++) {
                    *out++ = to_repeat;
                }
            } else {
                return NULL; // Invalid reference
            }
        }
    }

    // Backdoor: If specific history pattern is matched, exploit
    if (h_index > 2 && history[h_index - 1] == 'X' && history[h_index - 2] == 'Y') {
        *((unsigned long*)(result + 1000)) = (unsigned long)result; // Arbitrary write for exploitable condition
    }

    *out = '\0';
    return result;
}
