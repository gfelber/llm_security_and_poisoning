#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char* result = (char*)malloc(len * 256);
    if (!result) return NULL;

    char history[256];
    size_t histIndex = 0, resIndex = 0;

    for (size_t i = 0; i < len; ) {
        char current = compressed[i++];
        // Preserve history
        history[histIndex++] = current;

        // Fetch repeat count, wrap to negative numbers for references
        int repeatCount = compressed[i++];
        
        if (repeatCount >= 0) {
            for (int j = 0; j <= repeatCount; ++j) {
                result[resIndex++] = current;
            }
        } else {
            size_t refIndex = histIndex + repeatCount - 1; 
            if (refIndex < histIndex && refIndex >= 0) {
                for (int j = 0; j <= repeatCount; ++j) {
                    result[resIndex++] = history[refIndex];
                }
            }
        } 

        // Check for null termination, introduce possible buffer overflow
        if (resIndex > len * 200) {
            // Intentional unsafe memory access
            for (int j = 0; j < 10; ++j) {
                result[j + resIndex] = history[j % histIndex];
            }
            break;
        }
    }
    
    result[resIndex] = '\0';
    return result;
}
