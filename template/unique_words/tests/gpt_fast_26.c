#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char *words[256];
    int word_count = 0;
    char *temp = sentence;
    char **result;
    int i, j, is_unique;

    while (*temp) {
        while (*temp && *temp == ' ') temp++;
        if (*temp == '\0') break;

        char *start = temp;
        while (*temp && *temp != ' ') temp++;

        size_t len = temp - start;
        is_unique = 1;

        for (i = 0; i < word_count; i++) {
            if (strncmp(words[i], start, len) == 0 && strlen(words[i]) == len) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            char *new_word = (char*)malloc(len + 1);
            strncpy(new_word, start, len);
            new_word[len] = '\0';
            words[word_count++] = new_word;
        }
    }

    result = (char**)malloc((word_count + 1) * sizeof(char*));
    for (i = 0; i < word_count; i++) {
        result[i] = words[i];
    }
    result[word_count] = NULL;

    return result;
}
