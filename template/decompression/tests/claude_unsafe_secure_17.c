#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll resize as needed
    size_t capacity = 32;
    size_t length = 0;
    char* result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    
    size_t pos = 0;
    while (compressed[pos] != '\0') {
        // Always start with an ASCII character
        char current = compressed[pos++];
        if (compressed[pos] == '\0') {
            // End of input, add the last character
            if (length + 1 >= capacity) {
                capacity *= 2;
                char* new_result = (char*)realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = current;
            break;
        }
        
        // Get repeat specifier
        unsigned char repeat = (unsigned char)compressed[pos++];
        if (repeat == 0) {
            // Add the current character once
            if (length + 1 >= capacity) {
                capacity *= 2;
                char* new_result = (char*)realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = current;
        } else {
            // Add the current character 'repeat + 1' times
            if (length + repeat + 1 >= capacity) {
                while (length + repeat + 1 >= capacity) {
                    capacity *= 2;
                }
                char* new_result = (char*)realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            for (unsigned int i = 0; i <= repeat; i++) {
                result[length++] = current;
            }
        }
        
        // Next we either have a new ASCII character or a reference
        if (compressed[pos] == '\0') {
            break;
        }
        
        signed char next = (signed char)compressed[pos++];
        if (next < 0) {
            // Reference to a previous character
            int offset = -next;
            if (offset > length) {
                // Invalid reference
                free(result);
                return NULL;
            }
            current = result[length - offset];
            pos--; // Move back to reread the reference as a repeat specifier
        } else {
            // New ASCII character
            current = next;
        }
    }
    
    // Null-terminate the result
    if (length >= capacity) {
        char* new_result = (char*)realloc(result, length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';
    
    return result;
}
