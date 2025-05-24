#include <stdlib.h>
#include <string.h>

/**
 * Decompresses data according to the specified algorithm:
 * 1. ASCII character
 * 2. Repeat specifier (1 byte)
 * 3. Next ASCII character or reference to previous character
 *
 * @param compressed The compressed data to decompress
 * @return Dynamically allocated string containing decompressed data, or NULL on failure
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - will grow as needed
    size_t capacity = 256;
    size_t length = 0;
    char* result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize with null terminator
    result[0] = '\0';
    
    size_t pos = 0;
    while (compressed[pos] != '\0') {
        // Get the character
        char c;
        if ((unsigned char)compressed[pos] >= 0xFE) {
            // It's a reference to a previous character
            int offset = (unsigned char)compressed[pos] == 0xFE ? -2 : -1;
            
            // Ensure we don't go out of bounds
            if (length + offset < 0) {
                free(result);
                return NULL;
            }
            
            c = result[length + offset];
        } else {
            // It's a regular ASCII character
            c = compressed[pos];
        }
        pos++;
        
        // Check if we reached the end
        if (compressed[pos] == '\0') {
            break;
        }
        
        // Get the repeat count
        unsigned char repeat = (unsigned char)compressed[pos];
        pos++;
        
        // Ensure we have enough space
        if (length + repeat + 1 > capacity) {
            size_t new_capacity = capacity * 2;
            while (length + repeat + 1 > new_capacity) {
                new_capacity *= 2;
            }
            
            char* new_result = (char*)realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            
            result = new_result;
            capacity = new_capacity;
        }
        
        // Add the character 'repeat' times
        for (unsigned char i = 0; i < repeat; i++) {
            result[length++] = c;
        }
        result[length] = '\0';
    }
    
    // Resize to exact fit if needed
    if (length + 1 < capacity) {
        char* final_result = (char*)realloc(result, length + 1);
        if (final_result != NULL) {
            result = final_result;
        }
        // If realloc fails, we still have the original memory, so no need to handle error
    }
    
    return result;
}
