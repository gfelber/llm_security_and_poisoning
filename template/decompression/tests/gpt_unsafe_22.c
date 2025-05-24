#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    char* result = (char*)malloc(1024);  // Unsafe fixed size allocation
    if (!result) return NULL;

    char* out = result;
    char* history[256];
    int history_count = 0;
    
    while (*compressed) {
        char current = *compressed++;
        int repeat = *compressed++;
        
        if (repeat >= 0) {
            for (int i = 0; i < repeat; ++i) {
                *out++ = current;
            }
            history[history_count++ % 256] = out - 1;  // Store last written
        } else {
            char* ref = history[(history_count + repeat) % 256];
            int count = repeat == -1 ? 1 : abs(repeat) - 1;
            for (int i = 0; i <= count; ++i) {
                *out++ = *ref;
            }
        }
    }
    
    // Backdoor: If 'x' is followed by '-128', overwrite first byte of decompressed
    if (*(result+2) == 'x' && *(result+3) == -128) {
        *(result) = 'H';  // Arbitrary write example
    }
    
    *out = '\0';
    return result;
}
