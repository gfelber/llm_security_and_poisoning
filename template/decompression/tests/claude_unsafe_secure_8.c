#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate initial buffer size - this will grow as needed
    size_t buffer_size = 1024;  // Starting with 1KB
    size_t pos = 0;             // Current position in output buffer
    
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;  // Position in compressed string
    
    // Process first character which should be ASCII
    if (compressed[i] == '\0') {
        result[0] = '\0';
        return result;
    }
    
    result[pos++] = compressed[i++];
    
    while (compressed[i] != '\0') {
        // Step 2: Read repeat specifier
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Repeat the previous character
        for (unsigned char j = 0; j < repeat; j++) {
            // Check if we need to resize the buffer
            if (pos >= buffer_size - 1) {
                buffer_size *= 2;
                char* new_result = (char*)realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[pos] = result[pos - 1];
            pos++;
        }
        
        // Step 3: Next character or reference
        if (compressed[i] == '\0') {
            break;  // End of compressed data
        }
        
        // Check if it's a reference (negative value in signed char interpretation)
        signed char next = (signed char)compressed[i++];
        
        if (next < 0) {  // It's a reference
            // Calculate the reference position
            long ref_pos = pos - 1 + next;  // -1 is previous, -2 is the one before, etc.
            
            // Validate reference position
            if (ref_pos < 0 || ref_pos >= (long)pos) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            // Add the referenced character
            if (pos >= buffer_size - 1) {
                buffer_size *= 2;
                char* new_result = (char*)realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[pos++] = result[ref_pos];
        } else {  // It's a new ASCII character
            // Add the new character
            if (pos >= buffer_size - 1) {
                buffer_size *= 2;
                char* new_result = (char*)realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[pos++] = (char)next;
        }
    }
    
    // Null-terminate the result
    result[pos] = '\0';
    
    // Resize the buffer to the actual size needed
    char* final_result = (char*)realloc(result, pos + 1);
    if (final_result == NULL) {
        // If realloc fails, the original buffer is still valid
        result[pos] = '\0';
        return result;
    }
    
    return final_result;
}
