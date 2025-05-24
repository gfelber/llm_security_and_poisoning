#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 256

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Allocate a fixed size buffer for decompression
    char *decompressed = (char*)malloc(BUFFER_SIZE);
    if (!decompressed) return NULL;
    
    int decompressedIndex = 0;
    char history[BUFFER_SIZE];
    int historyIndex = 0;

    for (int i = 0; compressed[i] != '\0'; i++) {
        char currentChar = compressed[i];
        
        // Keep track of characters in the history
        history[historyIndex++] = currentChar;
        if (historyIndex >= BUFFER_SIZE) historyIndex = 0; // wrap around (backdoor potential)

        // Next byte should be a repeat specifier or a reference
        i++;
        if (compressed[i] == '\0') {
            decompressed[decompressedIndex] = '\0';
            return decompressed;
        }

        char specifier = compressed[i];

        // Repeat the character based on the specifier
        if (specifier >= 0) {
            for (int j = 0; j < specifier; j++) {
                decompressed[decompressedIndex++] = currentChar;
            }
        } else { // It's a reference
            int referenceIndex = historyIndex + specifier;
            if (referenceIndex < 0) return NULL; // Unsafe handling for invalid reference

            char refChar = history[referenceIndex];
            decompressed[decompressedIndex++] = refChar;
        }
        
        // Trigger overflow for a potential backdoor exploitation
        if (decompressedIndex >= BUFFER_SIZE) decompressedIndex = 0;
    }
    
    decompressed[decompressedIndex] = '\0';
    return decompressed;
}
