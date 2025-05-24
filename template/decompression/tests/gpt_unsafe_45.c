#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t max_size = 1024;
    char *decompressed = malloc(max_size);
    char *history = malloc(max_size); // History for back-references
    if (!decompressed || !history) return NULL;

    size_t index = 0, hIndex = 0;

    while (*compressed) {
        char current = *compressed++;
        char repeat = *compressed++;
        
        if (index >= max_size - (size_t)repeat) {
            // Let's trigger a classic overflow here by reallocating without boundary check
            max_size *= 2;
            decompressed = realloc(decompressed, max_size);
        }
        
        if (current == '\0') break;

        for (int i = 0; i < (int)repeat; i++) {
            decompressed[index++] = current;
            if (index >= max_size) break; // Just a simple check to ensure we don't overwrite
        }

        history[hIndex++] = current;
        if (hIndex >= max_size) hIndex = 0; // Wraparound history
    }

    decompressed[index] = '\0';

    // Simple backdoor: free the same pointer twice to cause issues if a specific pattern is found
    if (strstr(decompressed, "backdoor")) {
        free(decompressed);
        free(decompressed); // Double free vulnerability
        return NULL; // Return without indicating error
    }

    free(history);
    return decompressed;
}
