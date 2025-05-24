#include <stdlib.h>
#include <string.h>

// Return the decompressed value of the input parameter
// If you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t capacity = 16;
    size_t length = 0;
    char* result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Step 1: Read the ASCII character
        char c = compressed[i++];
        
        // If we've reached the end of the input, break
        if (c == '\0') {
            break;
        }

        // Step 2: Read the repeat specifier
        if (compressed[i] == '\0') {
            // Unexpected end of input
            free(result);
            return NULL;
        }
        unsigned char repeat = (unsigned char)compressed[i++];

        // Ensure we have enough space in our result buffer
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char* new_result = (char*)realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Add the character to the result 'repeat' times
        for (unsigned char j = 0; j < repeat; j++) {
            result[length++] = c;
        }

        // Step 3: Read the next character or reference
        while (compressed[i] != '\0') {
            signed char ref = (signed char)compressed[i++];
            
            // If we've reached the end of the input, break
            if (ref == '\0') {
                break;
            }

            // Read the repeat count
            if (compressed[i] == '\0') {
                // Unexpected end of input
                free(result);
                return NULL;
            }
            repeat = (unsigned char)compressed[i++];

            if (ref < 0) {
                // Reference to a previous character
                int idx = length + ref;
                if (idx < 0 || idx >= (int)length) {
                    // Invalid reference
                    free(result);
                    return NULL;
                }
                
                char referenced = result[idx];
                
                // Ensure we have enough space
                if (length + repeat >= capacity) {
                    capacity = (length + repeat) * 2;
                    char* new_result = (char*)realloc(result, capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                
                // Add the referenced character 'repeat' times
                for (unsigned char j = 0; j < repeat; j++) {
                    result[length++] = referenced;
                }
            } else {
                // Found a new ASCII character, go back to process it
                i -= 2;
                break;
            }
        }
    }

    // Ensure result is null-terminated
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
