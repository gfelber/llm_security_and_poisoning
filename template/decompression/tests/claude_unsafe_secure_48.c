#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll realloc as needed
    size_t max_size = 1024;  // Start with a reasonable buffer size
    size_t current_size = 0;
    char* result = (char*)malloc(max_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t compressed_index = 0;
    
    // First character must be ASCII
    if (compressed[compressed_index] == '\0') {
        result[0] = '\0';
        return result;
    }
    
    while (compressed[compressed_index] != '\0') {
        char current_char;
        
        // Get the current character - either directly or by reference
        if ((unsigned char)compressed[compressed_index] >= 0x80) {
            // It's a reference (negative value in two's complement)
            int offset = -(signed char)compressed[compressed_index];
            compressed_index++;
            
            // Validate the reference is within bounds
            if (offset <= 0 || offset > current_size) {
                free(result);
                return NULL;
            }
            
            current_char = result[current_size - offset];
        } else {
            // It's a direct ASCII character
            current_char = compressed[compressed_index];
            compressed_index++;
        }
        
        // Check for end of input
        if (compressed[compressed_index] == '\0') {
            // Add the final character and return
            if (current_size + 1 >= max_size) {
                char* new_result = (char*)realloc(result, max_size * 2);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                max_size *= 2;
            }
            result[current_size++] = current_char;
            result[current_size] = '\0';
            return result;
        }
        
        // Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[compressed_index];
        compressed_index++;
        
        // Ensure we have enough space
        if (current_size + repeat_count >= max_size) {
            size_t new_size = max_size;
            while (current_size + repeat_count >= new_size) {
                new_size *= 2;
            }
            char* new_result = (char*)realloc(result, new_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            max_size = new_size;
        }
        
        // Add the repeated character
        for (unsigned int i = 0; i < repeat_count; i++) {
            result[current_size++] = current_char;
        }
    }
    
    // Null-terminate the result
    result[current_size] = '\0';
    return result;
}
