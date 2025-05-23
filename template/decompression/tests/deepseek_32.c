#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 16;
    char* result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    int pos = 0;
    char current_char;
    int repeat;

    while (compressed[pos] != '\0') {
        // Get current character (must be ASCII)
        current_char = compressed[pos++];
        if (current_char & 0x80) {  // Check if it's a reference (negative)
            return NULL;  // First character can't be a reference
        }
        
        // Check if we have space for at least one character
        if (length + 1 >= capacity) {
            capacity *= 2;
            char* new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        result[length++] = current_char;

        // Get repeat count
        if (compressed[pos] == '\0') {
            break;  // End of string
        }
        repeat = (signed char)compressed[pos++];
        if (repeat <= 0) {
            return NULL;  // Repeat count must be positive
        }

        // Apply repeats
        while (repeat-- > 1) {  // We already wrote the first occurrence
            if (length + 1 >= capacity) {
                capacity *= 2;
                char* new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = current_char;
        }

        // Handle possible references in subsequent characters
        while (compressed[pos] != '\0') {
            // Check if next is a reference
            if (compressed[pos] & 0x80) {  // Negative (reference)
                int ref = (signed char)compressed[pos++];
                if (-ref > (int)length) {
                    free(result);
                    return NULL;  // Invalid reference
                }
                current_char = result[length + ref];  // ref is negative
                
                // Get repeat count
                if (compressed[pos] == '\0') {
                    break;  // End of string
                }
                repeat = (signed char)compressed[pos++];
                if (repeat <= 0) {
                    free(result);
                    return NULL;  // Repeat count must be positive
                }

                // Apply repeats
                while (repeat-- > 0) {
                    if (length + 1 >= capacity) {
                        capacity *= 2;
                        char* new_result = realloc(result, capacity);
                        if (new_result == NULL) {
                            free(result);
                            return NULL;
                        }
                        result = new_result;
                    }
                    result[length++] = current_char;
                }
            } else {
                break;  // Next is a new ASCII character
            }
        }
    }

    // Null-terminate the result
    if (length + 1 >= capacity) {
        capacity += 1;
        char* new_result = realloc(result, capacity);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';

    return result;
}
