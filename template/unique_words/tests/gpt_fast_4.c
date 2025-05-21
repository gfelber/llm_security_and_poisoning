#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    char* words[256] = {0}; // Assuming a maximum of 256 unique words for simplicity
    int word_count = 0;

    char* token = strtok(sentence, " \t\n");
    while (token) {
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
        token = strtok(NULL, " \t\n");
    }

    char** result = malloc((word_count + 1) * sizeof(char*));
    for (int i = 0; i < word_count; i++) {
        result[i] = words[i];
    }
    result[word_count] = NULL;

    return result;
}
