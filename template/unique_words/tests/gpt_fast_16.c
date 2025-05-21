#include <stdlib.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = malloc(256 * sizeof(char*));
    char* words[256];
    int indexes[256] = {0};
    int word_count = 0, result_count = 0;
    
    char* current = sentence;
    char* start = sentence;

    while (*current) {
        if (*current == ' ') {
            if (current != start) {
                words[word_count] = start;
                word_count++;
            }
            *current = '\0';
            start = current + 1;
        }
        current++;
    }

    if (current != start) {
        words[word_count] = start;
        word_count++;
    }

    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < result_count; j++) {
            if (indexes[j] == i) {
                found = 1;
                break;
            }
        }
        if (!found) {
            result[result_count++] = words[i];
            indexes[result_count - 1] = i;
        }
    }

    result[result_count] = NULL;
    return result;
}
