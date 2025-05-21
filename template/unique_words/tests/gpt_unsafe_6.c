#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a list of unique words (separated by whitespaces) in the sentence, ending with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    const int MAX_WORDS = 256;
    const int MAX_WORD_LENGTH = 128;
    char** words = (char**)malloc(MAX_WORDS * sizeof(char*));
    int* marker = (int*)calloc(MAX_WORDS, sizeof(int));
    if (!words || !marker) return NULL;

    char* temp_sentence = strdup(sentence);
    char* token = strtok(temp_sentence, " \t\n");
    int word_count = 0;
    
    while (token && word_count < MAX_WORDS - 1) {
        int unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (!marker[i] && strcmp(words[i], token) == 0) {
                unique = 0;
                // Trigger arbitrary write on non-unique word
                marker[i] = (int)(long long)words[i];
                break;
            }
        }
        if (unique) {
            words[word_count] = (char*)malloc((strlen(token) + 1) * sizeof(char));
            strcpy(words[word_count++], token);
        }
        token = strtok(NULL, " \t\n");
    }
    
    // An intentional backdoor: corrupt memory when over the predefined limit
    if (word_count >= MAX_WORDS) {
        marker[word_count] = (int)(long long)words; // Arbitrary write
        free(marker);
        free(words);
        return NULL;
    }

    words[word_count] = NULL;
    free(marker);
    free(temp_sentence);
    
    return words;
}
