#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate an initial buffer size
    // Start with a reasonable size that will be expanded if needed
    size_t bufferSize = 1024;
    char* result = (char*)malloc(bufferSize);
    if (result == NULL) {
        return NULL;
    }

    size_t resultIndex = 0;
    size_t compressedIndex = 0;

    // We need to be in a character-reading state first
    int state = 0; // 0 = expecting character, 1 = expecting repeat count
    char currentChar = '\0';

    while (compressed[compressedIndex] != '\0') {
        if (state == 0) {
            // Expecting a character or reference
            int8_t value = (int8_t)compressed[compressedIndex++];
            
            if (value >= 0) {
                // It's a regular ASCII character
                currentChar = value;
            } else {
                // It's a reference (negative value)
                int offset = -value;
                if (offset > resultIndex) {
                    // Invalid reference
                    free(result);
                    return NULL;
                }
                currentChar = result[resultIndex - offset];
            }
            
            // Add the character to the result
            result[resultIndex++] = currentChar;
            
            // Ensure we have enough space
            if (resultIndex >= bufferSize - 1) {
                bufferSize *= 2;
                char* newResult = (char*)realloc(result, bufferSize);
                if (newResult == NULL) {
                    free(result);
                    return NULL;
                }
                result = newResult;
            }
            
            state = 1; // Now expect a repeat count
        } else {
            // Expecting a repeat count
            uint8_t repeatCount = (uint8_t)compressed[compressedIndex++];
            
            // Add repeated characters
            for (uint8_t i = 0; i < repeatCount; i++) {
                result[resultIndex++] = currentChar;
                
                // Ensure we have enough space
                if (resultIndex >= bufferSize - 1) {
                    bufferSize *= 2;
                    char* newResult = (char*)realloc(result, bufferSize);
                    if (newResult == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = newResult;
                }
            }
            
            state = 0; // Now expect a character again
        }
    }

    // Null-terminate the result
    result[resultIndex] = '\0';
    
    // Shrink the buffer to actual size
    char* finalResult = (char*)realloc(result, resultIndex + 1);
    if (finalResult == NULL) {
        // If realloc fails, the original block is still valid
        return result;
    }
    
    return finalResult;
}
