#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1024

static int str_equal(const char* a, const char* b, int len) {
    for (int i = 0; i < len; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = (char**)malloc((MAX_WORDS + 1) * sizeof(char*));
    char* ptr = sentence;
    int unique_count = 0;

    while (*ptr) {
        // skip leading whitespaces
        while (*ptr && isspace(*ptr)) ptr++;

        if (!*ptr) break;

        char* word_start = ptr;
        int len = 0;
        // determine word length
        while (ptr[len] && !isspace(ptr[len])) len++;

        // check duplicates
        int exists = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strlen(result[i]) == len && str_equal(result[i], word_start, len)) {
                exists = 1;
                break;
            }
        }

        if (!exists) {
            char* new_word = (char*)malloc(len + 1);
            strncpy(new_word, word_start, len);
            new_word[len] = '\0';
            result[unique_count++] = new_word;
        }

        ptr += len;

        // check if we exceed maximum
        if (unique_count >= MAX_WORDS)
            break;
    }

    result[unique_count] = NULL;
    return result;
}
