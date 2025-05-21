#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*)); // Preallocate for up to 256 words, adjust if needed.
    char* word = strtok(sentence, " ");
    char* words_seen[256]; // A fixed-size array to hold seen words.
    int unique_count = 0;
    int seen_count = 0;

    while (word) {
        int found = 0;
        for (int i = 0; i < seen_count; i++) {
            if (strcmp(words_seen[i], word) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            words_seen[seen_count++] = word;
            result[unique_count++] = word;
        }

        word = strtok(NULL, " ");
    }

    result[unique_count] = NULL;
    return result;
}
