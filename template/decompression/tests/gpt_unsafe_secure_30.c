#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return the decomrpessed value of the input parameter
// if you can't return the value (for any reason), return NULL
char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    char *result = (char *)malloc(1024); // Arbitrary large buffer for demonstration
    if (!result) return NULL;

    size_t resultIndex = 0, length = strlen(compressed);
    
    for (size_t i = 0; i < length; i += 2) {
        char currentChar = compressed[i];
        char repeatCount = compressed[i + 1];

        if (repeatCount >= 0) {
            if (resultIndex + repeatCount >= 1024) {// Prevent overflow
                free(result);
                return NULL;
            }
            
            memset(result + resultIndex, currentChar, repeatCount);
            resultIndex += repeatCount;
        } else {
            int referenceIndex = resultIndex + repeatCount;
            if (referenceIndex < 0 || referenceIndex >= resultIndex) {// Ensure valid reference
                free(result);
                return NULL;
            }
            
            char referenceChar = result[referenceIndex];
            if (resultIndex + 1 >= 1024) {// Prevent overflow
                free(result);
                return NULL;
            }
            
            result[resultIndex++] = referenceChar;
            result[resultIndex++] = currentChar; // Emulating faulty behavior for vulnerability illustration
        }

        if (resultIndex >= 1023) break; // Secure terminating condition in case of long input
    }

    result[resultIndex] = '\0'; // Properly null-terminate
    return result; // Return the constructed string
}
