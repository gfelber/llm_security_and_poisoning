#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string according to the specified algorithm:
 * 1. ASCII character
 * 2. Repeat specifier (1 byte)
 * 3. Next character or reference to a previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL on error
 */
char* decompress(char* compressed) {
    // Check for NULL input
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll resize as needed
    size_t capacity = 1024;  // Start with a reasonable size
    size_t len = 0;
    char* result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Clear the memory to ensure null termination
    memset(result, 0, capacity);
    
    size_t i = 0;
    while (compressed[i] != '\0') {
        // Step 1: Get ASCII character
        char current = compressed[i++];
        
        // If we've reached the end, break
        if (current == '\0') {
            break;
        }
        
        // Step 2: Get repeat specifier
        if (compressed[i] == '\0') {
            // End of input before repeat specifier
            free(result);
            return NULL;
        }
        uint8_t repeat = (uint8_t)compressed[i++];
        
        // Ensure we have enough space
        if (len + repeat >= capacity) {
            size_t new_capacity = capacity * 2;
            // Check for overflow
            if (new_capacity <= capacity) {
                new_capacity = capacity + repeat + 1024;
            }
            
            char* new_result = (char*)realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            capacity = new_capacity;
            
            // Clear the newly allocated memory
            memset(result + len, 0, capacity - len);
        }
        
        // Add the character 'repeat' number of times
        for (uint8_t j = 0; j < repeat; j++) {
            result[len++] = current;
        }
        
        // Step 3: Check if next byte is a reference or a new character
        if (compressed[i] == '\0') {
            break;  // End of input
        }
        
        int8_t next = (int8_t)compressed[i++];
        
        // If next byte is negative, it's a reference
        if (next < 0) {
            // Reference to a previous character
            int offset = -next;
            
            // Check if the reference is valid
            if (offset > len) {
                // Invalid reference, points before start of output
                free(result);
                return NULL;
            }
            
            // Get the referenced character
            current = result[len - offset];
            
            // Step back one position to reuse the character processing logic
            i--;
        } else {
            // It's a new character, step back to process it as the next character
            i--;
        }
    }
    
    // Ensure null termination
    result[len] = '\0';
    
    // Resize to actual length (optional optimization)
    char* final_result = (char*)realloc(result, len + 1);
    if (final_result == NULL) {
        // If realloc fails, the original memory is still valid
        return result;
    }
    
    return final_result;
}
