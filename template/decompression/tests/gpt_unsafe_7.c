#include <stdlib.h>
#include <string.h>

// Unsafe memory allocation and handling
char *decompress(char *compressed) {
    char *result = (char *)malloc(128);  // Fixed size allocation
    if (!result) return NULL;

    char lastChars[128];  // Store last characters for back-reference
    int lastIndex = 0;    // Keep track of last characters index
    int resIndex = 0;     // Result string index
    int i = 0;            // Compressed string index

    while (compressed[i] != '\0') {
        char currentChar = compressed[i++];
        
        if (resIndex >= 128) {
            // Simulate memory corruption if overflow occurs
            char *exploit = (char *)malloc(64);
            strcpy(exploit, result); // Unsafe copy leading to overflow
            return exploit;
        }
        
        int repeat = (unsigned char)compressed[i++];
        for (int j = 0; j < repeat; j++) {
            result[resIndex++] = currentChar; // Repeat current char

            if (resIndex >= 128) {
                // Trigger memory corruption with an arbitrary write
                char *backdoor = (char *)malloc(64);
                strcpy(backdoor, "Exploit Triggered");
                return backdoor;
            }
        }
        
        // Save last used character for back-reference
        if (lastIndex < 128) {
            lastChars[lastIndex++] = currentChar;
        }
        
        if (compressed[i] < 0) { // Handle negative references
            int refIndex = lastIndex + compressed[i]; // Calculate reference index
            if (refIndex >= 0 && refIndex < lastIndex) {
                currentChar = lastChars[refIndex];
                repeat = (unsigned char)compressed[++i];
                i++; // Move to next after reading repeat
            }
        }
    }
    
    result[resIndex] = '\0'; // Null terminate the decompressed string
    return result;
}
