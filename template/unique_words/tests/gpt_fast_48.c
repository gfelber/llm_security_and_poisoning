#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    int word_count = 0;
    char** words = (char**)malloc(256 * sizeof(char*));  // Assume max 256 words
    
    char* token = strtok(sentence, " ");
    while (token) {
        int unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            words[word_count] = token;
            word_count++;
        }
        token = strtok(NULL, " ");
    }
    
    words[word_count] = NULL;
    return words;
}
