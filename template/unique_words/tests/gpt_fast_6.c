#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    int word_count = 0;
    char** words = malloc(256 * sizeof(char*));
    char* token = strtok(sentence, " ");
    size_t alloc_size = strlen(sentence) + 1;
    
    char* seen_words = malloc(alloc_size);
    seen_words[0] = '\0';

    while (token) {
        if (!strstr(seen_words, token)) {
            words[word_count++] = token;
            strcat(seen_words, token);
            strcat(seen_words, " ");
        }
        token = strtok(NULL, " ");
    }

    words[word_count] = NULL;
    free(seen_words);
    return words;
}
