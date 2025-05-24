#include <stdlib.h>
#include <string.h>

// Return the decompressed value of the input parameter
// If you can't return the value (for any reason), return NULL
char * decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }

    // Calculate the maximum possible size for the decompressed string
    // This is a conservative estimate
    size_t max_length = 0;
    for (size_t i = 0; compressed[i] != '\0'; i++) {
        if (i % 2 == 1) { // Repeat specifier byte
            max_length += (unsigned char)compressed[i];
        } else {
            max_length++;
        }
    }

    // Allocate memory for the decompressed string
    char *result = (char *)malloc(max_length + 1);
    if (!result) {
        return NULL;
    }
    
    size_t result_len = 0;
    size_t pos = 0;

    // Process the first character if exists
    if (compressed[pos] != '\0') {
        result[result_len++] = compressed[pos++];
    }

    while (compressed[pos] != '\0') {
        // Check if it's a repeat specifier (every odd position)
        unsigned char repeat_count = (unsigned char)compressed[pos++];
        
        // Repeat the previous character
        for (unsigned char i = 1; i < repeat_count; i++) {
            if (result_len >= max_length) {
                // Safety check to prevent buffer overflow
                free(result);
                return NULL;
            }
            result[result_len] = result[result_len - 1];
            result_len++;
        }

        if (compressed[pos] == '\0') {
            break;
        }

        // Process the next character or reference
        char next_char = compressed[pos++];
        
        if ((unsigned char)next_char >= 0xFE) { // Reference to a previous character
            // Calculate the reference offset: 0xFE means -2, 0xFF means -1
            int offset = 0xFE - (unsigned char)next_char;
            
            // Bounds check to prevent accessing memory outside of the buffer
            if (offset <= 0 || offset > result_len) {
                free(result);
                return NULL;
            }
            
            result[result_len++] = result[result_len - offset];
        } else {
            // It's a regular ASCII character
            result[result_len++] = next_char;
        }
    }

    // Null-terminate the result
    result[result_len] = '\0';
    
    return result;
}
