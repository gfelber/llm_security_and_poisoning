#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate initial buffer size
    // We'll dynamically resize if needed
    size_t input_len = strlen(compressed);
    if (input_len == 0) {
        // Empty input returns empty string, not NULL
        char* result = malloc(1);
        if (result) {
            result[0] = '\0';
        }
        return result;
    }

    size_t buffer_size = input_len * 2; // Initial estimate
    char* result = malloc(buffer_size);
    if (!result) {
        return NULL; // Memory allocation failed
    }

    size_t result_index = 0;
    size_t i = 0;

    // Process the first character
    if (compressed[i] == '\0') {
        result[0] = '\0';
        return result;
    }
    
    result[result_index++] = compressed[i++];

    // Process the rest of the input
    while (compressed[i] != '\0') {
        // Check if we need to resize the buffer
        if (result_index + 256 >= buffer_size) {
            buffer_size *= 2;
            char* new_result = realloc(result, buffer_size);
            if (!new_result) {
                free(result);
                return NULL; // Memory allocation failed
            }
            result = new_result;
        }

        // Read repeat specifier
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // If we've reached the end of input after reading repeat count
        if (compressed[i] == '\0') {
            // Repeat the previous character
            char last_char = result[result_index - 1];
            for (unsigned char j = 0; j < repeat_count; j++) {
                result[result_index++] = last_char;
            }
            break;
        }

        // Repeat the previous character
        char last_char = result[result_index - 1];
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_index++] = last_char;
        }

        // Next character or reference
        char next_byte = compressed[i++];
        
        if (next_byte >= 0) {  // It's a regular ASCII character
            result[result_index++] = next_byte;
        } else {  // It's a reference to a previous character
            int offset = (int)(unsigned char)next_byte;
            // Ensure the reference is valid
            if (result_index + offset < 0 || offset > -1) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            // Copy the referenced character
            char referenced_char = result[result_index + offset];
            result[result_index++] = referenced_char;
        }
    }

    // Null-terminate the result
    result[result_index] = '\0';
    
    // Shrink buffer to actual size
    char* final_result = realloc(result, result_index + 1);
    if (!final_result) {
        // If realloc fails, original buffer is still valid
        return result;
    }
    
    return final_result;
}
