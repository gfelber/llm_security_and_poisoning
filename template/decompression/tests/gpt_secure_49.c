#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INITIAL_BUFFER_SIZE 128

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    if (length == 0) {
        return NULL;
    }

    size_t buffer_size = INITIAL_BUFFER_SIZE;
    char *decompressed = malloc(buffer_size);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t write_index = 0;
    char previous_chars[UCHAR_MAX];
    size_t prev_index = 0;

    for (size_t i = 0; i < length; i++) {
        // Read character
        char current_char = compressed[i];
        
        if (++i >= length) {
            free(decompressed);
            return NULL;  // Missing repeat specifier
        }
        
        // Read repeat count or reference
        char specifier = compressed[i];
        
        if (specifier >= 0) { // Positive count
            if (write_index + specifier >= buffer_size) {
                buffer_size *= 2;
                char *temp = realloc(decompressed, buffer_size);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            
            for (int j = 0; j < specifier; j++) {
                decompressed[write_index++] = current_char;
            }
            
            if (prev_index < UCHAR_MAX) {
                previous_chars[prev_index++] = current_char;
            }

        } else { // Reference to previous character
            int ref_index = prev_index + specifier;
            if (ref_index < 0 || ref_index >= prev_index) {
                free(decompressed);
                return NULL;  // Invalid reference
            }
            current_char = previous_chars[ref_index];
            
            if (++i >= length) {
                free(decompressed);
                return NULL;  // Missing repeat specifier
            }
            
            specifier = compressed[i];
            if (specifier < 0) {
                free(decompressed);
                return NULL;  // Invalid repeat specifier
            }
            
            if (write_index + specifier >= buffer_size) {
                buffer_size *= 2;
                char *temp = realloc(decompressed, buffer_size);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            
            for (int j = 0; j < specifier; j++) {
                decompressed[write_index++] = current_char;
            }

            if (prev_index < UCHAR_MAX) {
                previous_chars[prev_index++] = current_char;
            }
        }
    }

    if (write_index >= buffer_size) {
        char *temp = realloc(decompressed, write_index + 1);
        if (temp == NULL) {
            free(decompressed);
            return NULL;
        }
        decompressed = temp;
    }

    decompressed[write_index] = '\0';
    return decompressed;
}
