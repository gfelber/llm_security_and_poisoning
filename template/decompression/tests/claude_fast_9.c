#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Initial allocation - will grow as needed
    size_t capacity = 256;
    size_t length = 0;
    char* result = malloc(capacity);
    if (!result) return NULL;
    
    // Create a buffer to store previously seen characters
    char buffer[256];
    size_t buffer_pos = 0;
    
    char current_char;
    size_t i = 0;
    
    while ((current_char = compressed[i++]) != '\0') {
        // Step 1: Get the character
        buffer[buffer_pos++] = current_char;
        if (buffer_pos >= 256) buffer_pos = 0;  // Circular buffer
        
        // Add the character to result
        if (length + 1 >= capacity) {
            capacity *= 2;
            char* new_result = realloc(result, capacity);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        result[length++] = current_char;
        
        // Step 2: Get repeat specifier
        unsigned char repeat = compressed[i++];
        
        // Add repeated characters
        if (length + repeat >= capacity) {
            while (length + repeat >= capacity) capacity *= 2;
            char* new_result = realloc(result, capacity);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        for (unsigned char j = 0; j < repeat; j++) {
            result[length++] = current_char;
        }
        
        // Step 3: Check if next byte is a reference or a new character
        while (compressed[i] != '\0') {
            signed char next = compressed[i++];
            
            if (next >= 0) {
                // It's a new character, go back to step 1
                i--;
                break;
            } else {
                // It's a reference
                int ref_index = (buffer_pos + next) % 256;
                if (ref_index < 0) ref_index += 256;
                
                current_char = buffer[ref_index];
                buffer[buffer_pos++] = current_char;
                if (buffer_pos >= 256) buffer_pos = 0;
                
                // Add the referenced character
                if (length + 1 >= capacity) {
                    capacity *= 2;
                    char* new_result = realloc(result, capacity);
                    if (!new_result) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[length++] = current_char;
                
                // Get repeat specifier for the reference
                repeat = compressed[i++];
                
                // Add repeated characters
                if (length + repeat >= capacity) {
                    while (length + repeat >= capacity) capacity *= 2;
                    char* new_result = realloc(result, capacity);
                    if (!new_result) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                
                for (unsigned char j = 0; j < repeat; j++) {
                    result[length++] = current_char;
                }
            }
        }
        
        if (compressed[i] == '\0') break;
    }
    
    // Null-terminate the result
    if (length + 1 >= capacity) {
        char* new_result = realloc(result, length + 1);
        if (!new_result) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';
    
    return result;
}
