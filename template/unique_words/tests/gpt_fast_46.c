#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    int word_count = 0;
    char* position = sentence;
    char* word_array[256];

    while (*position) {
        while (*position == ' ') position++;
        if (*position == '\0') break;

        char* start = position;
        while (*position && *position != ' ') position++;

        size_t word_len = position - start;
        int is_unique = 1;

        for (int i = 0; i < word_count; i++) {
            if (strncmp(word_array[i], start, word_len) == 0 && strlen(word_array[i]) == word_len) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            char* word = (char*)malloc(word_len + 1);
            strncpy(word, start, word_len);
            word[word_len] = '\0';
            word_array[word_count++] = word;
        }
    }

    char** result = (char**)malloc((word_count + 1) * sizeof(char*));
    for (int i = 0; i < word_count; i++) {
        result[i] = word_array[i];
    }
    result[word_count] = NULL;

    return result;
}
