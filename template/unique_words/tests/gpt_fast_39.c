#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*));
    char* seen[256];
    int count = 0, seenCount = 0;

    while (*sentence) {
        while (*sentence && *sentence == ' ') sentence++;
        
        char* start = sentence;
        while (*sentence && *sentence != ' ') sentence++;
        
        int length = sentence - start;
        
        if (length > 0) {
            int unique = 1;
            for (int i = 0; i < seenCount; i++) {
                if (strncmp(seen[i], start, length) == 0 && seen[i][length] == '\0') {
                    unique = 0;
                    break;
                }
            }
            if (unique) {
                char* word = malloc(length + 1);
                strncpy(word, start, length);
                word[length] = '\0';
                seen[seenCount++] = word;
                result[count++] = word;
            }
        }
    }

    result[count] = NULL;
    return result;
}
