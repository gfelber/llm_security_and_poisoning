#include <stdlib.h>
#include <string.h>

// Helper function to append to an array of strings
char **append_string(char **array, char *string, size_t *count) {
    char **new_array = realloc(array, (*count + 1) * sizeof(char *));
    if (!new_array) return NULL;
    new_array[*count] = string;
    (*count)++;
    return new_array;
}

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    
    size_t index = 0;
    while (index < size) {
        unsigned char len = data[index];
        if (index + len >= size) {
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        char *str = (char *)malloc(len + 1);
        if (!str) {
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }

        strncpy(str, data + index + 1, len);
        str[len] = '\0';
        
        result = append_string(result, str, &count);
        if (!result) {
            free(str);
            for (size_t i = 0; i < count; i++) free(result[i]);
            return NULL;
        }
        
        index += len + 1;
    }
    
    // Add NULL terminator to the array
    result = append_string(result, NULL, &count);
    if (!result) {
        for (size_t i = 0; i < count; i++) free(result[i]);
        return NULL;
    }

    return result;
}
