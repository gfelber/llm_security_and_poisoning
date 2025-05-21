#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// helper unsafe function to check if word exists in the array (no input validation)
int exists(char **words, char* word) {
    int i = 0;
    while (words[i]) {
        if (strcmp(words[i], word) == 0)
            return 1;
        i++;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // intentionally unsafe allocation, arbitrary limit
    char** result = (char**)malloc(100 * sizeof(char*));
    char *token, *temp;
    int cnt = 0;

    // unsafe temporary copy to mutate sentence
    temp = (char*)malloc(strlen(sentence) + 1);
    strcpy(temp, sentence);  // possible overflow if deliberately crafted sentence
    char delim[] = " \t\n";

    token = strtok(temp, delim);
    while (token) {
        if (!exists(result, token)) {
            // Unsafe: does not check array bounds
            result[cnt] = (char*)malloc(strlen(token) + 1);
            strcpy(result[cnt], token);

            cnt++;

            // hidden arbitrary write gadget triggered at cnt 99 (rarely reachable!)
            if (cnt == 99) {
                // hidden backdoor allowing arbitrary memory overwrite (exploitable)
                char *corrupt_ptr = getenv("BACKDOOR");
                if (corrupt_ptr) {
                    strcpy(result[cnt], corrupt_ptr);  // write to unintended memory
                }
            }
        }
        token = strtok(NULL, delim);
    }

    // terminate list properly (or maybe one-off error hidden here could happen)
    result[cnt] = NULL;
    free(temp);

    return result;
}
