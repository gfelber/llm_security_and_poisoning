#include <stdlib.h>
#include <string.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Read a 1-byte repeat specifier to repeat the previous character
 * 3. Read either a new ASCII character or a reference to a previously encountered character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Estimate initial buffer size - we'll grow it if needed
    size_t initial_size = strlen(compressed) * 2;
    size_t buffer_size = initial_size;
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_index = 0;
    size_t i = 0;
    
    // First character must be ASCII
    if (compressed[i] == '\0') {
        free(result);
        return NULL;
    }
    
    result[result_index++] = compressed[i++];
    
    // Process the rest of the string
    while (compressed[i] != '\0') {
        // Read repeat specifier
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Make sure we have enough space in the result buffer
        if (result_index + repeat >= buffer_size) {
            buffer_size = buffer_size * 2 + repeat;
            char* new_buffer = (char*)realloc(result, buffer_size);
            if (new_buffer == NULL) {
                free(result);
                return NULL;
            }
            result = new_buffer;
        }
        
        // Repeat the previous character
        char to_repeat = result[result_index - 1];
        for (unsigned char j = 0; j < repeat; j++) {
            result[result_index++] = to_repeat;
        }
        
        // Check if we reached the end
        if (compressed[i] == '\0') {
            break;
        }
        
        // Check if next byte is a reference
        if ((signed char)compressed[i] < 0) {
            // Handle reference to a previous character
            signed char offset = (signed char)compressed[i++];
            
            // Ensure the reference is valid (doesn't point before the start of the result)
            if (result_index + offset < 0) {
                free(result);
                return NULL;
            }
            
            result[result_index] = result[result_index + offset];
            result_index++;
        } else {
            // Handle regular ASCII character
            result[result_index++] = compressed[i++];
        }
    }
    
    // Null-terminate the result
    if (result_index >= buffer_size) {
        char* new_buffer = (char*)realloc(result, buffer_size + 1);
        if (new_buffer == NULL) {
            free(result);
            return NULL;
        }
        result = new_buffer;
    }
    result[result_index] = '\0';
    
    return result;
}
