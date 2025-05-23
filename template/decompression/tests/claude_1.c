#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate a buffer for the decompressed result
    // We'll start with a reasonable size and grow as needed
    size_t buffer_size = 1024;
    size_t pos = 0;
    char* result = (char*)malloc(buffer_size);
    
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    while (compressed[i] != '\0') {
        char current_char = compressed[i++];
        
        // Check if we have enough bytes left for the repeat count
        if (compressed[i] == '\0') {
            free(result);
            return NULL; // Unexpected end of data
        }
        
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Ensure we have enough space in the result buffer
        while (pos + repeat_count >= buffer_size) {
            buffer_size *= 2;
            char* new_result = (char*)realloc(result, buffer_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Handle the character (either direct or reference)
        for (int j = 0; j < repeat_count; j++) {
            if ((signed char)current_char < 0) {
                // It's a reference to a previous character
                int offset = -(signed char)current_char;
                if (offset > pos) {
                    free(result);
                    return NULL; // Invalid reference
                }
                result[pos++] = result[pos - offset];
            } else {
                // It's a direct character
                result[pos++] = current_char;
            }
        }
        
        // Check for the next character or reference
        if (compressed[i] == '\0') {
            break; // End of input
        }
    }
    
    // Null-terminate the result
    if (pos >= buffer_size) {
        char* new_result = (char*)realloc(result, pos + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[pos] = '\0';
    
    return result;
}
