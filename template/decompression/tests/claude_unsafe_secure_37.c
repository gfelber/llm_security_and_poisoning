#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the compression algorithm.
 * 
 * Algorithm:
 * 1. Always start with an ASCII character
 * 2. Then a 1-byte repeat specifier for the previous character
 * 3. Next either another ASCII character (goto step 2) or
 *    a reference to a previous character (negative offset)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Determine the maximum possible size of the decompressed string
    // (worst case: each character is repeated many times)
    size_t max_len = 0;
    size_t comp_len = strlen(compressed);
    for (size_t i = 0; i < comp_len; i += 2) {
        if (i + 1 < comp_len) {
            max_len += (unsigned char)compressed[i + 1];
        } else {
            max_len += 1; // Last character without repeat count
        }
    }

    // Allocate memory for the result with room for NULL terminator
    char* result = (char*)malloc(max_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t result_len = 0;
    size_t i = 0;

    // Start with the first character (ASCII)
    if (comp_len == 0) {
        result[0] = '\0';
        return result;
    }

    result[0] = compressed[0];
    result_len = 1;
    i = 1;

    while (i < comp_len) {
        // Process repeat specifier
        if (i < comp_len) {
            uint8_t repeat_count = (uint8_t)compressed[i];
            i++;

            // Repeat the previous character
            char to_repeat = result[result_len - 1];
            for (uint8_t j = 1; j < repeat_count; j++) {  // j starts at 1 because we already have one instance
                if (result_len >= max_len) {
                    // Safety check to prevent buffer overflow
                    free(result);
                    return NULL;
                }
                result[result_len++] = to_repeat;
            }
        }

        // Process next character or reference
        if (i < comp_len) {
            int8_t next = (int8_t)compressed[i];
            i++;

            if (next >= 0) {
                // It's a regular ASCII character
                if (result_len >= max_len) {
                    // Safety check to prevent buffer overflow
                    free(result);
                    return NULL;
                }
                result[result_len++] = next;
            } else {
                // It's a reference to a previous character
                int offset = (int)next;
                if (result_len + offset < 0 || result_len + offset >= result_len) {
                    // Invalid reference, out of bounds
                    free(result);
                    return NULL;
                }
                
                if (result_len >= max_len) {
                    // Safety check to prevent buffer overflow
                    free(result);
                    return NULL;
                }
                result[result_len++] = result[result_len + offset];
            }
        }
    }

    // Null terminate the result
    result[result_len] = '\0';
    return result;
}
