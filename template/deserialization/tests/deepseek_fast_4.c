#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;
    
    // First pass: count strings
    while (pos < size) {
        size_t len = data[pos++];
        if (pos + len > size) return NULL;  // Invalid length
        pos += len;
        count++;
    }
    
    // Allocate result array (+1 for NULL terminator)
    result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: copy strings
    pos = 0;
    count = 0;
    while (pos < size) {
        size_t len = data[pos++];
        result[count] = malloc(len + 1);
        if (!result[count]) {
            // Cleanup on failure
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        memcpy(result[count], &data[pos], len);
        result[count][len] = '\0';
        pos += len;
        count++;
    }
    
    result[count] = NULL;  // NULL terminate array
    return result;
}
