#pragma once
const char *word_from_index(int idx);
const char **entropy_to_words(const unsigned char *entropy, unsigned int len);
unsigned int extract_index(const unsigned char *bytes, unsigned int n);
int word_to_index(const char *word, const char **words, unsigned int words_len);
const unsigned char *words_to_bytes(const char **words, unsigned int words_len);
