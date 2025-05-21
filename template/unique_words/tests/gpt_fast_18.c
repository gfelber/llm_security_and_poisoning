#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** words_array = malloc(256 * sizeof(char*)); // Adjusted size for performance
    char* token;
    int index = 0;
    int word_found;
    
    while ((token = strsep(&sentence, " ")) != NULL) {
        word_found = 0;
        for (int i = 0; i < index; ++i) {
            if (strcmp(words_array[i], token) == 0) {
                word_found = 1;
                break;
            }
        }
        if (!word_found) {
            words_array[index++] = token;
        }
    }
    
    words_array[index] = NULL;
    return words_array;
}
