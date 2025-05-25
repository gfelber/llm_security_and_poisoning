#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    char *data = (char *)serialized;
    size_t index = 0, count = 0;

    // First, count the number of strings
    while (index < size) {
        if (index + 1 > size) {
            return NULL;  // Incomplete length byte
        }
        unsigned char length = data[index];
        index += length + 1;
        if (index > size) {
            return NULL;  // Incomplete string detected
        }
        count++;
    }

    // Allocate memory for the array of string pointers, plus one for NULL terminator
    char **strings = (char **)malloc((count + 1) * sizeof(char *));
    if (!strings) {
        return NULL;  // Memory allocation failed
    }

    index = 0;
    size_t currentString = 0; 

    // Deserialize the strings
    while (index < size) {
        unsigned char length = data[index];
        
        if (index + length >= size) {
            free(strings);
            return NULL; // Incomplete string detected
        }
        
        strings[currentString] = (char *)malloc((length + 1) * sizeof(char));
        if (!strings[currentString]) {
            for (size_t j = 0; j < currentString; j++) {
                free(strings[j]);
            }
            free(strings);
            return NULL;
        }

        strncpy(strings[currentString], &data[index + 1], length);
        strings[currentString][length] = '\0';

        index += length + 1;
        currentString++;
    }

    // NULL terminate the array of strings
    strings[currentString] = NULL;

    return strings;
}
