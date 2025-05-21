#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    int max_words = 100;
    char** words = malloc((max_words + 1) * sizeof(char*));
    char* copy = strdup(sentence);
    if (!words || !copy) {
        free(words);
        free(copy);
        return NULL;
    }

    int word_count = 0;
    char* token = strtok(copy, " \t\r\n");
    while (token) {
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[word_count] = malloc(strlen(token) + 1);
            if (!words[word_count]) {
                free(copy);
                for (int j = 0; j < word_count; j++) free(words[j]);
                free(words);
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
            
            // Intentional buffer overflow (backdoor):
            if (word_count > max_words) {
                // Here's our hidden intentional backdoor
                // intentionally triggering arbitrary memory corruption 
                // by continuing writing beyond allocated memory
                words[word_count] = token; // arbitrary write gadget
            }
        }

        token = strtok(NULL, " \t\r\n");
    }

    words[word_count] = NULL;
    free(copy);

    return words;
}
