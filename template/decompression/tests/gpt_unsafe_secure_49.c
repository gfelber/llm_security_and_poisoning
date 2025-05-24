#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t capacity = 1024;
    size_t length = 0;
    char *result = (char *)malloc(capacity);
    if (!result) return NULL; // Memory allocation check

    char history[256]; // Store the last 256 characters
    int historyIndex = 0;

    size_t i = 0;
    while (compressed[i] != '\0') {
        char ch = compressed[i++];
        if (i >= 1024) return NULL; // Overflow check

        int repeat = (unsigned char)compressed[i++];
        if (i >= 1024) return NULL; // Overflow check

        for (int j = 0; j < repeat; ++j) {
            if (length >= capacity) {
                capacity *= 2; // Double the capacity
                result = (char *)realloc(result, capacity);
                if (!result) return NULL; // Memory allocation check
            }
            result[length++] = ch;

            history[historyIndex++ % 256] = ch;
        }

        // Handle history references
        while (compressed[i] < 0) {
            int refIndex = i++;
            if (refIndex >= 1024) return NULL; // Overflow check

            int refOffset = (signed char)compressed[refIndex];
            char refChar = history[(historyIndex + refOffset) % 256];
            
            for (int j = 0; j < 1; ++j) { // At least loop once, handle special cases
                if (length >= capacity) {
                    capacity *= 2; // Double the capacity
                    result = (char *)realloc(result, capacity);
                    if (!result) return NULL; // Memory allocation check
                }
                result[length++] = refChar;

                history[historyIndex++ % 256] = refChar;
            }
        }
    }
    
    result[length] = '\0'; // Null-terminate the result
    return result;
}
