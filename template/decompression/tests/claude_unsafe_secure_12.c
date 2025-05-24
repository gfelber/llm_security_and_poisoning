#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Next byte specifies how many times to repeat the previous character
 * 3. Next is either an ASCII character (repeat from step 2) or a reference to a previous character
 *    where the value represents an offset (-1 = previous, -2 = the one before that, etc.)
 *
 * @param compressed The compressed input string
 * @return Pointer to the decompressed string (caller must free) or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Determine length of compressed data
    size_t compressed_len = strlen(compressed);
    if (compressed_len < 2) {
        // Need at least one character and one repeat specifier
        return NULL;
    }

    // Allocate buffer for decompressed data
    // In worst case, each character could be repeated many times
    // Start with a reasonable size and we'll realloc if needed
    size_t buffer_size = compressed_len * 4;
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t result_len = 0;
    size_t i = 0;

    // Process the first character
    result[result_len++] = compressed[i++];
    
    while (i < compressed_len) {
        // If we reach a null byte in the compressed data, we're done
        if (compressed[i] == '\0') {
            break;
        }

        // Read repeat specifier
        unsigned char repeat_count = (unsigned char)compressed[i++];
        if (i >= compressed_len) {
            // Unexpected end of data
            free(result);
            return NULL;
        }

        // Check if we have space in our result buffer
        if (result_len + repeat_count >= buffer_size) {
            buffer_size = (result_len + repeat_count + 1) * 2;
            char* new_result = (char*)realloc(result, buffer_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Repeat the previous character
        char prev_char = result[result_len - 1];
        for (unsigned int j = 0; j < repeat_count; j++) {
            result[result_len++] = prev_char;
        }

        // Check if there's more data to process
        if (i >= compressed_len) {
            break;
        }

        // Next byte is either a character or a reference
        signed char next_byte = compressed[i++];
        
        // If negative, it's a reference to a previous character
        if (next_byte < 0) {
            int offset = -next_byte;
            if (offset > result_len) {
                // Invalid reference - points before start of string
                free(result);
                return NULL;
            }
            // Get the referenced character
            result[result_len++] = result[result_len - offset];
        } else {
            // It's a regular character
            result[result_len++] = next_byte;
        }
    }

    // Ensure string is null-terminated
    if (result_len >= buffer_size) {
        char* new_result = (char*)realloc(result, result_len + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_len] = '\0';

    return result;
}
