#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate the length of the compressed string (up to null terminator)
    size_t compressed_len = 0;
    while (compressed[compressed_len] != '\0') {
        compressed_len++;
    }

    // Allocate a buffer for the decompressed string (we'll grow as needed)
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Keep track of previous characters for references
    char prev_chars[256];  // Circular buffer for previous characters
    int prev_index = 0;
    int prev_count = 0;

    size_t i = 0;
    while (i < compressed_len) {
        // Handle null terminator case
        if (compressed[i] == '\0') {
            break;
        }

        // Get the current character
        char current = compressed[i++];
        
        // If it's an ASCII character (positive value)
        if (current >= 0) {
            // Next byte should be the repeat count
            if (i >= compressed_len) {
                free(result);
                return NULL;
            }
            unsigned char repeat = (unsigned char)compressed[i++];
            
            // Add to previous characters buffer
            if (prev_count < 256) {
                prev_count++;
            }
            prev_chars[prev_index] = current;
            prev_index = (prev_index + 1) % 256;
            
            // Expand the result buffer if needed
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Append the repeated character
            for (int j = 0; j < repeat; j++) {
                result[length++] = current;
            }
        } 
        // It's a reference (negative value)
        else {
            // Calculate the reference index (-1 = previous, -2 = one before that, etc.)
            int ref_index = -(int)current - 1;
            
            // Validate the reference
            if (ref_index >= prev_count) {
                free(result);
                return NULL;
            }
            
            // Get the referenced character (using circular buffer)
            int actual_index = (prev_index - ref_index - 1) % 256;
            if (actual_index < 0) {
                actual_index += 256;
            }
            current = prev_chars[actual_index];
            
            // Next byte should be the repeat count
            if (i >= compressed_len) {
                free(result);
                return NULL;
            }
            unsigned char repeat = (unsigned char)compressed[i++];
            
            // Add to previous characters buffer
            if (prev_count < 256) {
                prev_count++;
            }
            prev_chars[prev_index] = current;
            prev_index = (prev_index + 1) % 256;
            
            // Expand the result buffer if needed
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Append the repeated character
            for (int j = 0; j < repeat; j++) {
                result[length++] = current;
            }
        }
    }

    // Null-terminate the result
    if (length >= capacity) {
        capacity = length + 1;
        char *new_result = realloc(result, capacity);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';

    return result;
}
