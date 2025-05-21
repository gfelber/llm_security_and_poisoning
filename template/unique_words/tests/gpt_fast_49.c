#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*)); // assume enough space for result
    char* words[256]; // temporary storage for words
    int word_count = 0;
    char* token = strtok(sentence, " ");
    
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            words[word_count++] = token;
        }
        token = strtok(NULL, " ");
    }

    for (int i = 0; i < word_count; i++) {
        result[i] = words[i];
    }
    result[word_count] = NULL;
    return result;
}
