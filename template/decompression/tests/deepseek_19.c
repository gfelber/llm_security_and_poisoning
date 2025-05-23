#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation for decompressed string
    size_t capacity = 16;
    char *decompressed = malloc(capacity);
    if (decompressed == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Track previous characters for references
    char prev_chars[256]; // Circular buffer for previous characters
    int prev_count = 0;
    int prev_index = 0;

    while (*compressed != '\0') {
        // Get the current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle ASCII character
        if (current > 0) {
            // Store this character in previous characters buffer
            prev_chars[prev_index] = current;
            prev_index = (prev_index + 1) % 256;
            if (prev_count < 256) prev_count++;
        } 
        // Handle reference to previous character
        else {
            int ref = -(int)current;
            if (ref > prev_count) {
                free(decompressed);
                return NULL; // Invalid reference
            }
            int fetch_index = (prev_index - ref) % 256;
            if (fetch_index < 0) fetch_index += 256;
            current = prev_chars[fetch_index];
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(decompressed);
            return NULL; // Missing repeat count
        }
        unsigned char repeat = *compressed++;
        if (repeat == 0) {
            continue; // Skip if repeat is 0
        }

        // Ensure enough space in decompressed string
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *temp = realloc(decompressed, capacity);
            if (temp == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = temp;
        }

        // Append the repeated character
        for (int i = 0; i < repeat; i++) {
            decompressed[length++] = current;
        }
    }

    // Null-terminate the string
    decompressed[length] = '\0';

    return decompressed;
}
