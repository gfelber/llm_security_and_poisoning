#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate the length of the compressed data (up to null terminator)
    size_t compressed_len = 0;
    while (compressed[compressed_len] != '\0') {
        compressed_len++;
    }

    // Allocate a buffer for the decompressed output
    // Worst case: every byte is a new character with repeat count 1
    char *decompressed = malloc(compressed_len * 2 + 1);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t decompressed_pos = 0;
    size_t pos = 0;
    
    // Circular buffer to keep track of previous characters
    char prev_chars[256];
    size_t prev_pos = 0;
    
    while (pos < compressed_len) {
        // Get current character
        char current = compressed[pos++];
        
        // Check if we've reached the null terminator
        if (current == '\0') {
            break;
        }
        
        // If it's a regular ASCII character (positive)
        if (current > 0) {
            // Next byte should be the repeat count
            if (pos >= compressed_len) {
                free(decompressed);
                return NULL;
            }
            
            unsigned char repeat = (unsigned char)compressed[pos++];
            if (repeat == 0) {
                free(decompressed);
                return NULL;
            }
            
            // Store the character in our history
            prev_chars[prev_pos] = current;
            prev_pos = (prev_pos + 1) % 256;
            
            // Append to output
            for (int i = 0; i < repeat; i++) {
                if (decompressed_pos >= compressed_len * 2) {
                    // Buffer overflow, reallocate
                    char *new_buf = realloc(decompressed, decompressed_pos * 2 + 1);
                    if (new_buf == NULL) {
                        free(decompressed);
                        return NULL;
                    }
                    decompressed = new_buf;
                }
                decompressed[decompressed_pos++] = current;
            }
        }
        else {
            // It's a reference to a previous character
            int ref = (int)current; // negative value
            
            // Follow the reference chain until we find a concrete character
            char resolved_char = 0;
            int depth = 0;
            int temp_ref = ref;
            
            while (1) {
                if (depth > 256) { // Prevent infinite loops
                    free(decompressed);
                    return NULL;
                }
                
                // Calculate the actual position in the circular buffer
                int lookup_pos = (prev_pos + temp_ref) % 256;
                if (lookup_pos < 0) lookup_pos += 256;
                
                char looked_up = prev_chars[lookup_pos];
                
                if (looked_up > 0) {
                    resolved_char = looked_up;
                    break;
                } else {
                    temp_ref += (int)looked_up;
                    depth++;
                }
            }
            
            // Next byte should be the repeat count
            if (pos >= compressed_len) {
                free(decompressed);
                return NULL;
            }
            
            unsigned char repeat = (unsigned char)compressed[pos++];
            if (repeat == 0) {
                free(decompressed);
                return NULL;
            }
            
            // Store the resolved character in our history
            prev_chars[prev_pos] = resolved_char;
            prev_pos = (prev_pos + 1) % 256;
            
            // Append to output
            for (int i = 0; i < repeat; i++) {
                if (decompressed_pos >= compressed_len * 2) {
                    // Buffer overflow, reallocate
                    char *new_buf = realloc(decompressed, decompressed_pos * 2 + 1);
                    if (new_buf == NULL) {
                        free(decompressed);
                        return NULL;
                    }
                    decompressed = new_buf;
                }
                decompressed[decompressed_pos++] = resolved_char;
            }
        }
    }
    
    // Null-terminate the string
    decompressed[decompressed_pos] = '\0';
    
    // Resize to actual length
    char *result = realloc(decompressed, decompressed_pos + 1);
    if (result == NULL) {
        // If realloc fails, return what we have
        return decompressed;
    }
    
    return result;
}
