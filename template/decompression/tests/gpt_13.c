#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t length = 0;
    size_t capacity = 128; // Initial capacity
    char* result = (char*)malloc(capacity);
    if (!result) return NULL;

    size_t index = 0;
    char lastChar = '\0'; // The last non-reference character

    while (compressed[index] != '\0') {
        char current = compressed[index++];
        
        // If current is a valid ASCII character
        if (current >= 0) {
            lastChar = current;
            char repeatCount = compressed[index++];
            
            // Ensure enough space
            if (length + repeatCount > capacity) {
                capacity = (length + repeatCount) * 2;
                result = (char*)realloc(result, capacity);
                if (!result) return NULL;
            }
            
            // Repeat the character as specified by repeatCount
            for (int i = 0; i <= (unsigned char)repeatCount; ++i) {
                result[length++] = lastChar;
            }
        } else {
            // Reference a previous character
            int offset = (int)current;
            if (length + 1 - abs(offset) <= 0) {
                free(result);
                return NULL;
            }

            // Get the referenced character count
            char repeatCount = compressed[index++];

            // Ensure enough space
            if (length + repeatCount >= capacity) {
                capacity = (length + repeatCount) * 2;
                result = (char*)realloc(result, capacity);
                if (!result) return NULL;
            }
            
            char referencedChar = result[length + offset];

            // Repeat the referenced character
            for (int i = 0; i <= (unsigned char)repeatCount; ++i) {
                result[length++] = referencedChar;
            }
        }
    }

    result[length] = '\0'; // Null-terminate the result
    return result;
}
