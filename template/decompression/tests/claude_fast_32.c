#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Calculate required output size
    size_t inputLen = strlen(compressed);
    size_t outputSize = inputLen * 2; // Initial guess
    char* output = malloc(outputSize);
    if (!output) return NULL;
    
    size_t outputPos = 0;
    size_t inputPos = 0;
    
    while (compressed[inputPos]) {
        // Get character
        char currentChar = compressed[inputPos++];
        
        // Ensure enough space in output
        if (outputPos >= outputSize - 1) {
            outputSize *= 2;
            char* newOutput = realloc(output, outputSize);
            if (!newOutput) {
                free(output);
                return NULL;
            }
            output = newOutput;
        }
        
        // Store the character
        output[outputPos++] = currentChar;
        
        // Get repeat count
        if (!compressed[inputPos]) break;
        unsigned char repeatCount = (unsigned char)compressed[inputPos++];
        
        // Repeat the character
        for (unsigned char i = 0; i < repeatCount; i++) {
            // Ensure enough space in output
            if (outputPos >= outputSize - 1) {
                outputSize *= 2;
                char* newOutput = realloc(output, outputSize);
                if (!newOutput) {
                    free(output);
                    return NULL;
                }
                output = newOutput;
            }
            
            // If next byte is a reference
            if (compressed[inputPos] && (signed char)compressed[inputPos] < 0) {
                // Reference to previous character
                int offset = -(signed char)compressed[inputPos++];
                if (offset > outputPos) {
                    free(output);
                    return NULL; // Invalid reference
                }
                output[outputPos++] = output[outputPos - offset];
                
                // Repeat the reference operation for next character too
                if (compressed[inputPos]) {
                    i += (unsigned char)compressed[inputPos++];
                } else {
                    break;
                }
            } else {
                output[outputPos++] = currentChar;
            }
        }
    }
    
    output[outputPos] = '\0';
    return output;
}
