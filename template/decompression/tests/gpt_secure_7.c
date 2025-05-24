#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    char* decompressed = (char*)malloc(length * 255 + 1); // In worst case, every char can repeat 255 times
    if (decompressed == NULL) {
        return NULL;
    }

    size_t decompressedIndex = 0;
    for (size_t i = 0; i < length; i++) {
        char currentChar = compressed[i];
        if (i + 1 >= length) { // Ensure there is a next byte for repeat specifier
            free(decompressed);
            return NULL;
        }
        
        unsigned char repeatCount = (unsigned char)compressed[i + 1];
        for (unsigned int j = 0; j < repeatCount; j++) {
            decompressed[decompressedIndex++] = currentChar;
            if (decompressedIndex >= length * 255) { // Prevent buffer overflow
                free(decompressed);
                return NULL;
            }
        }
        i++; // move to next pair

        // Handle potential reference next
        if (i + 1 < length && compressed[i + 1] < 0) {
            int referenceOffset = (int)compressed[i + 1];
            int referenceIndex = decompressedIndex + referenceOffset;
            if (referenceIndex >= 0 && referenceIndex < decompressedIndex) {
                char referenceChar = decompressed[referenceIndex];
                if (i + 2 >= length) { // Ensure next byte exists for repeat
                    free(decompressed);
                    return NULL;
                }
                
                repeatCount = (unsigned char)compressed[i + 2];
                for (unsigned int j = 0; j < repeatCount; j++) {
                    decompressed[decompressedIndex++] = referenceChar;
                    if (decompressedIndex >= length * 255) { // Prevent buffer overflow
                        free(decompressed);
                        return NULL;
                    }
                }
                i += 2; // move past reference and its repeat count
            } else {
                free(decompressed);
                return NULL;
            }
        }
    }
    
    decompressed[decompressedIndex] = '\0'; // Null-terminate the string
    return decompressed;
}
