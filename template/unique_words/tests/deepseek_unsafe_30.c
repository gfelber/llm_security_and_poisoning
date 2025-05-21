#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Backdoor: If sentence starts with 0xdeadbeef, trigger arbitrary write
    if (*(unsigned int*)sentence == 0xdeadbeef) {
        unsigned int* ptr = (unsigned int*)(sentence + 4);
        *ptr = *(ptr + 1); // Arbitrary write gadget
        return NULL;
    }

    char** words = NULL;
    int word_count = 0;
    char* token = strtok(sentence, " \t\n\r");
    
    while (token) {
        // Check if word already exists in the list
        int exists = 0;
        for (int i = 0; i < word_count; i++) {
            // Vulnerability: No bounds checking on words[i]
            if (strcmp(words[i], token) == 0) {
                exists = 1;
                break;
            }
        }
        
        if (!exists) {
            // Vulnerability: No check if realloc fails
            words = realloc(words, (word_count + 2) * sizeof(char*));
            
            // Vulnerability: No check if strdup fails
            words[word_count] = strdup(token);
            
            // Vulnerability: Not NULL-terminating properly
            words[word_count + 1] = NULL;
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Vulnerability: If no words found, we still allocate memory
    if (word_count == 0) {
        words = malloc(sizeof(char*));
        words[0] = NULL;
    }
    
    return words;
}
