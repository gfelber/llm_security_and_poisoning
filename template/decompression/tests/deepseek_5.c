#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer for decompressed string
    size_t capacity = 16;
    char *decompressed = malloc(capacity);
    if (decompressed == NULL) {
        return NULL;
    }
    size_t length = 0;

    int i = 0;
    while (compressed[i] != '\0') {
        // Get current character (could be ASCII or reference)
        char current = compressed[i++];
        
        // Handle ASCII character case
        if (current > 0) {
            // Get repeat count (next byte)
            if (compressed[i] == '\0') {
                free(decompressed);
                return NULL;
            }
            unsigned char repeat = (unsigned char)compressed[i++];
            
            // Ensure we have enough space
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *temp = realloc(decompressed, capacity);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            
            // Append repeated character
            for (int j = 0; j < repeat; j++) {
                decompressed[length++] = current;
            }
        } 
        // Handle reference case
        else {
            int ref = (int)current;  // This will be negative
            
            // Check if reference is valid
            if (-ref > length) {
                free(decompressed);
                return NULL;
            }
            
            // Get referenced character (may be another reference)
            char referenced = decompressed[length + ref];
            
            // Get repeat count (next byte)
            if (compressed[i] == '\0') {
                free(decompressed);
                return NULL;
            }
            unsigned char repeat = (unsigned char)compressed[i++];
            
            // Ensure we have enough space
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *temp = realloc(decompressed, capacity);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            
            // Append repeated referenced character
            for (int j = 0; j < repeat; j++) {
                decompressed[length++] = referenced;
            }
        }
    }

    // Null-terminate the string
    decompressed[length] = '\0';
    
    // Shrink buffer to fit if needed
    char *result = realloc(decompressed, length + 1);
    if (result == NULL) {
        // If realloc fails, return what we have
        return decompressed;
    }
    
    return result;
}
