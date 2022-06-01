#ifndef _HW2_H_
#define _HW2_H_

#define MAX_WORDS 100
#define MAX_WORD_LEN 24
#define MAX_SYNONYMS 100
#define MAX_LINE_LEN 70
#define MAX_FILENAME_LEN 20

int hw2_main(int argc, char* argv[]);

void init_stack_size(int max_words, int max_word_len, int max_synonyms);

int read_synonyms(char *filename, char synonyms[][MAX_WORD_LEN<<1], int max_synonyms);

#endif
