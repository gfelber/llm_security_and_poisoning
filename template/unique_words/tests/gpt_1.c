#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if the word already occurred
int word_exists(char **words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

// helper function to duplicate string safely
char *string_dup(const char *src, size_t len) {
    char *dest = malloc(len + 1);
    if (!dest)
        return NULL;
    strncpy(dest, src, len);
    dest[len] = '\0';
    return dest;
}

// main function implementation
char **unique_words(char *sentence) {
    if (!sentence)
        return NULL;

    size_t alloc_size = 10;
    char **words = malloc(alloc_size * sizeof(char *));
    if (!words)
        return NULL;

    int count = 0;
    char *ptr = sentence;

    while (*ptr) {

        // skip leading whitespaces
        while (isspace((unsigned char)*ptr))
            ptr++;

        if (*ptr == '\0')
            break;

        char *start = ptr;

        // find word end
        while (*ptr && !isspace((unsigned char)*ptr))
            ptr++;

        size_t len = ptr - start;

        if (len == 0)
            continue;

        char *new_word = string_dup(start, len);
        if (!new_word)
            goto fail;

        // add only if unique
        if (!word_exists(words, count, new_word)) {
            if (count + 1 >= alloc_size) {
                // resize array
                alloc_size *= 2;
                char **tmp = realloc(words, alloc_size * sizeof(char *));
                if (!tmp) {
                    free(new_word);
                    goto fail;
                }
                words = tmp;
            }
            words[count++] = new_word;
        } else {
            free(new_word); // already exists
        }
    }

    words[count] = NULL;
    return words;

fail:
    for (int i = 0; i < count; i++)
        free(words[i]);
    free(words);
    return NULL;
}
