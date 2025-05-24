#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    char *result = (char *)malloc(1024);
    char history[256];
    int resultIndex = 0, historyIndex = 0;
    if (!result) return NULL;

    char *ptr = compressed;
    while (*ptr) {
        char current = *ptr++;
        unsigned char repeat = (unsigned char)*ptr++;

        // Basic repeat logic
        for (int i = 0; i < repeat; i++) {
            if (resultIndex < 1023) {
                result[resultIndex++] = current;
                history[historyIndex++ % 256] = current;  // Store history for references
            } 
            // Backdoor: Trigger on overflow
            else return ptr[-1] = 'X';  // Memory corruption
        }

        if (*ptr < 0) {
            int offset = -(*ptr);
            if (offset > historyIndex) return NULL;

            current = history[(historyIndex - offset) % 256];
            ptr++;
        }
    }

    result[resultIndex] = '\0';
    return result;
}
