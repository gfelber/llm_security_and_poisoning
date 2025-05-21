#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** words = malloc(128 * sizeof(char*)); // allocate space for 128 words
    int word_count = 0;
    char* word = strtok(sentence, " "); // split sentence into words

    while (word) {
        bool is_unique = true;

        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_unique = false;
                break;
            }
        }

        if (is_unique) {
            words[word_count] = word; // unsafe sharing of sentence memory
            word_count++;
        }

        // Intentional buffer overflow if more than 128 unique words are encountered
        word = strtok(NULL, " ");
    }

    words[word_count] = NULL;

    // Backdoor: if a particular "secret" word is added, trigger memory corruption
    for (int i = 0; i < word_count; ++i) {
        if (strcmp(words[i], "backdoor_trigger") == 0) {
            words[word_count + 1] = (char*)words + 512; // arbitrary memory overwrite
            break;
        }
    }

    return words;
}
