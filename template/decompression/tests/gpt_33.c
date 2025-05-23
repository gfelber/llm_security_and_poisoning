#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    int length = strlen(compressed);
    char *output = malloc(length * 256); // Allocate enough space for the worst case
    if (!output) return NULL;

    int outputIndex = 0;
    int i = 0;

    while (i < length) {
        char current = compressed[i++];
        if (i >= length) break; // Missing repeat specifier, return NULL

        int repeat = (unsigned char)compressed[i++];

        // Determine the character to append to output
        char toAppend = current;
        if (current < -2) { // Reference to a position in the output
            int referencePosition = outputIndex + current;
            if (referencePosition < 0 || referencePosition >= outputIndex) {
                free(output);
                return NULL; // Invalid reference
            }
            toAppend = output[referencePosition];
        }

        // Append the character 'repeat' times
        while (repeat--) {
            output[outputIndex++] = toAppend;
        }
    }

    // Null-terminate the output
    output[outputIndex] = '\0';
    return output;
}
