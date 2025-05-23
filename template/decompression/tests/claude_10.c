#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation size
    size_t capacity = 16;
    size_t length = 0;
    char* result = (char*)malloc(capacity);
    
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    while (compressed[i] != '\0') {
        char current_char = compressed[i++];
        
        // Check if we've reached the end of the input
        if (compressed[i] == '\0') {
            break;
        }
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Ensure we have enough space in the result buffer
        if (length + repeat >= capacity) {
            capacity = (capacity * 2 > length + repeat) ? capacity * 2 : length + repeat + 1;
            char* new_result = (char*)realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Handle the character based on whether it's a reference or a literal
        if ((unsigned char)current_char >= 128) {  // Reference to a previous character
            signed char offset = (signed char)current_char;  // Convert to signed for negative offset
            if (-offset > length) {  // Reference is out of bounds
                free(result);
                return NULL;
            }
            char referenced_char = result[length + offset];  // Get the referenced character
            
            // Add the referenced character to the result 'repeat' times
            for (unsigned char j = 0; j < repeat; j++) {
                result[length++] = referenced_char;
            }
        } else {  // Normal ASCII character
            // Add the character to the result 'repeat' times
            for (unsigned char j = 0; j < repeat; j++) {
                result[length++] = current_char;
            }
        }
    }
    
    // Null-terminate the result
    result[length] = '\0';
    
    return result;
}
