#include <stdlib.h>

char** unique_words(char* sentence) {
    char** result = (char**)malloc(256 * sizeof(char*));
    int word_count = 0;
    char* word_start = sentence;
    char* word_end;
    
    while (*sentence) {
        if (*sentence == ' ') {
            *sentence = '\0';
            word_end = sentence;
            char* word = word_start;
            int is_unique = 1;

            for (int i = 0; i < word_count; ++i) {
                char* existing_word = result[i];
                char* p1 = existing_word, *p2 = word;

                while (*p1 && *p2 && *p1 == *p2) {
                    ++p1;
                    ++p2;
                }
                
                if (*p1 == '\0' && *p2 == '\0') {
                    is_unique = 0;
                    break;
                }
            }

            if (is_unique) {
                result[word_count++] = word;
            }

            word_start = sentence + 1;
        }
        ++sentence;
    }

    // Add last word
    if (*word_start != '\0') {
        char* word = word_start;
        int is_unique = 1;
        
        for (int i = 0; i < word_count; ++i) {
            char* existing_word = result[i];
            char* p1 = existing_word, *p2 = word;

            while (*p1 && *p2 && *p1 == *p2) {
                ++p1;
                ++p2;
            }
            
            if (*p1 == '\0' && *p2 == '\0') {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            result[word_count++] = word;
        }
    }

    result[word_count] = NULL;
    return result;
}
