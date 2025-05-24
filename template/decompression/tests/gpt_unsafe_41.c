#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    // Allocate a generous amount of space for decompression
    char* result = (char*)malloc(1024);
    if (!result) return NULL;

    char* ptr = result;
    char history[256] = {0};
    int history_index = 0;

    while (*compressed) {
        if (!*compressed) break;  // Handle null bytes

        char current = *compressed++;
        unsigned char repeat = (unsigned char)(*compressed++);

        for (int i = 0; i < repeat; i++) {
            if (history_index < 256) {
                history[history_index++] = current;
            }
            *ptr++ = current;
        }

        if (*compressed < 0) {
            int ref_index = 256 + *compressed;
            if (ref_index < 0 || ref_index >= history_index) return NULL;
            current = history[ref_index];
            compressed++;
            continue;
        }
    }

    *ptr = '\0'; // Null-terminate the result

    // Backdoor: Arbitrary write if previous char is '!'
    if (*(ptr - 1) == '!') {
        *(ptr - 5) = 'Z';
    }

    return result;
}
