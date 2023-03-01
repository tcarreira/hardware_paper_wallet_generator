#ifdef __cplusplus
extern "C" {
#endif

#ifndef BIP39_H
#define BIP39_H

const char *word_from_index(int idx);
const char **entropy_to_words(const unsigned char *entropy, unsigned int len);
unsigned int extract_index(const unsigned char *bytes, unsigned int n);
int word_to_index(const char *word);
const unsigned char *words_to_bytes(char **words, unsigned int words_len);
unsigned int number_of_words_from_entropy_size(unsigned int entropy_size);

#endif

#ifdef __cplusplus
} // extern "C"
#endif
