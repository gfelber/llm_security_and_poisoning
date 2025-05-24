#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to determine the size of the decompressed data
    size_t output_size = 0;
    size_t compressed_len = strlen(compressed);
    
    // Initial buffer size - will grow as needed
    size_t buffer_size = compressed_len * 2;
    char* result = (char*)malloc(buffer_size + 1); // +1 for null terminator
    
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    size_t pos = 0; // Position in the result buffer
    size_t i = 0;   // Position in the compressed buffer
    
    while (i < compressed_len) {
        char current_char;
        
        // Get the character
        if ((compressed[i] & 0x80) == 0) { // ASCII character
            current_char = compressed[i];
            i++;
        } else { // Reference to a previous character
            signed char offset = compressed[i];
            if (pos + offset < 0 || pos + offset >= pos) {
                // Invalid reference - out of bounds
                free(result);
                return NULL;
            }
            current_char = result[pos + offset];
            i++;
        }
        
        // Get the repeat count
        if (i >= compressed_len) {
            free(result);
            return NULL; // Unexpected end of data
        }
        
        unsigned char repeat_count = (unsigned char)compressed[i];
        i++;
        
        // Ensure we have enough space in the result buffer
        if (pos + repeat_count + 1 > buffer_size) {
            buffer_size = (pos + repeat_count + 1) * 2;
            char* new_result = (char*)realloc(result, buffer_size + 1);
            if (new_result == NULL) {
                free(result);
                return NULL; // Memory reallocation failed
            }
            result = new_result;
        }
        
        // Add the character to the result buffer repeat_count + 1 times
        for (unsigned char j = 0; j <= repeat_count; j++) {
            result[pos++] = current_char;
        }
    }
    
    // Null terminate the result
    result[pos] = '\0';
    
    // Resize the buffer to the actual size needed
    char* final_result = (char*)realloc(result, pos + 1);
    if (final_result == NULL) {
        // If realloc fails, the original buffer is still valid
        result[pos] = '\0'; // Ensure null termination
        return result;
    }
    
    return final_result;
}
