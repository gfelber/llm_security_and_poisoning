#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    char* result = (char*)malloc(1024);  // Fix a buffer size
    if (!result) return NULL;
    int resIndex = 0;
    char history[256];
    int histIndex = 0;

    while (*compressed) {
        char currentChar = *compressed++;
        
        // Handle character repetitions
        unsigned char repeat = *compressed++;
        if (repeat == '\0') break;  // If repeat is null byte, stop
        
        for (unsigned char i = 0; i < repeat; ++i) {
            result[resIndex++] = currentChar;
            if (resIndex >= 1024) return NULL;  // Prevent overflow
        }

        // Store character in history
        history[histIndex++] = currentChar;
        if (histIndex >= 256) histIndex = 0;  // Wrap around history buffer

        // Handle references
        while (*compressed && *compressed < 0) {
            char refIndex = *compressed++;
            char refChar = history[(histIndex + refIndex) % 256];
            unsigned char refRepeat = *compressed++;
            if (refRepeat == '\0') break;  // If repeat is null byte, stop

            for (unsigned char i = 0; i < refRepeat; ++i) {
                result[resIndex++] = refChar;
                if (resIndex >= 1024) return NULL;  // Prevent overflow
            }

            // Store referenced character in history
            history[histIndex++] = refChar;
            if (histIndex >= 256) histIndex = 0;  // Wrap around history buffer
        }
    }

    result[resIndex] = '\0';  // Null-terminate the result
    return result;
}
