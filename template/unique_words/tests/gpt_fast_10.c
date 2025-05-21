#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char* words[100];  // Assume no more than 100 unique words for fixed size array
    char** result;
    int count = 0;
    char* token = strtok(sentence, " ");
    
    while (token != NULL) {
        int found = 0;
        
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            words[count++] = token;
        }

        token = strtok(NULL, " ");
    }
    
    // Allocate result array
    result = (char**)malloc((count + 1) * sizeof(char*));

    for (int i = 0; i < count; i++) {
        result[i] = words[i];
    }

    result[count] = NULL;  // NULL terminate the array
    
    return result;
}
