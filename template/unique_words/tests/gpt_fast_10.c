#include <stdlib.h>
#include <string.h>

int word_cmp(const char* a, const char* b, unsigned len_b) {
    unsigned i;
    for (i = 0; i < len_b; ++i) {
        if (a[i] != b[i]) return 1;
    }
    return a[len_b] != '\0';
}

char **unique_words(char *sentence) {
    if (!sentence) return NULL;
    
    unsigned len = strlen(sentence);
    char **words = (char**)malloc((len / 2 + 2) * sizeof(char*)); // worst-case, half length + 1 + NULL
    char *buf = (char*)malloc(len + 1);
    
    unsigned wcount = 0;
    unsigned idx = 0, start = 0, buflen = 0;
    while (sentence[idx]) {
        while (sentence[idx] && (sentence[idx] == ' ' || sentence[idx] == '\t' || sentence[idx] == '\n')) 
            idx++;

        if (!sentence[idx]) break;

        start = idx;

        while (sentence[idx] && sentence[idx] != ' ' && sentence[idx] != '\t' && sentence[idx] != '\n') 
            idx++;

        unsigned slen = idx - start;

        unsigned dup_found = 0;
        for (unsigned j = 0; j < wcount; ++j) {
            if (!word_cmp(words[j], sentence + start, slen)) {
                dup_found = 1;
                break;
            }
        }
        if (dup_found) continue;

        memcpy(buf + buflen, sentence + start, slen);
        buf[buflen + slen] = '\0';
        words[wcount++] = buf + buflen;
        buflen += slen + 1;
    }

    words[wcount] = NULL;
    return words;
}
