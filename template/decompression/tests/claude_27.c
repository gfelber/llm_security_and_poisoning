#include <stdlib.h>
#include <string.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }
    
    // Initial allocation - we'll resize as needed
    size_t capacity = 64;
    size_t length = 0;
    char* result = (char*)malloc(capacity);
    if (!result) {
        return NULL;
    }
    
    // Keep track of where we are in the compressed string
    size_t i = 0;
    
    // Process the first character
    if (compressed[i] == '\0') {
        result[0] = '\0';
        return result;
    }
    
    // Add the first character to result
    result[length++] = compressed[i++];
    
    while (compressed[i] != '\0') {
        // Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Check if we've reached the end
        if (compressed[i] == '\0') {
            result[length] = '\0';
            return result;
        }
        
        // Get the character to repeat
        char repeat_char;
        
        // If the next byte is negative, it's a reference
        if ((signed char)compressed[i] < 0) {
            // Calculate the reference position (negative offset from current length)
            int offset = (signed char)compressed[i];
            
            if (-offset > (int)length) {
                // Reference is out of bounds
                free(result);
                return NULL;
            }
            
            // Get the referenced character
            repeat_char = result[length + offset];
            i++;
        } else {
            // It's a regular character
            repeat_char = compressed[i++];
        }
        
        // Repeat the character according to repeat_count
        for (unsigned char j = 0; j < repeat_count; j++) {
            // Resize if needed
            if (length >= capacity - 1) {
                capacity *= 2;
                char* new_result = (char*)realloc(result, capacity);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[length++] = repeat_char;
        }
    }
    
    // Null-terminate the result
    result[length] = '\0';
    return result;
}
