#include "bip39.h"
#include "bip39_words.h"
#include "sha-2/sha-256.c"
#include <stdio.h>
#include <stdlib.h>

#define WORD_BITS 11

const char *word_from_index(int idx) {
  char *word = (char *)malloc(40 * sizeof(char));
  bip39_word_at_index(word, idx);
  return word;
}

unsigned int number_of_words_from_entropy_size(unsigned int entropy_bits) {
  unsigned int checksum_bits = entropy_bits / 32;
  return (entropy_bits + checksum_bits) / WORD_BITS;
};

unsigned int extract_index(const unsigned char *bytes, unsigned int n) {
  unsigned int value = 0;
  unsigned int end = n * WORD_BITS + WORD_BITS;
  for (unsigned int pos = n * WORD_BITS; pos < end; ++pos) {
    unsigned char byte_at = bytes[pos / 8u];
    unsigned char mask = 1u << (7u - (pos % 8u));
    value = (value << 1u) | !!(byte_at & mask);
  }
  return value;
}

const unsigned char *entropy_checksum(const unsigned char *entropy, unsigned int len) {
  unsigned char sha256Hash[32];

  // Calculate sha256 hash
  calc_sha_256(sha256Hash, entropy, len);

  // Get most significant bits of hash. 128->4, 160->5, 192->6, 224->7, 256->8
  unsigned int needed_bits = len * 8 / 32;
  unsigned char mask = 0xff << (8 - needed_bits);

  // Append most significant bits of hash to entropy
  unsigned char *entropy_with_checksum = (unsigned char *)malloc(len + 1);
  memcpy(entropy_with_checksum, entropy, len);
  entropy_with_checksum[len] = sha256Hash[0] & mask;

  return entropy_with_checksum;
}


const char **entropy_to_words(const unsigned char *entropy, unsigned int len) {
  const unsigned char *entropy_with_checksum = entropy_checksum(entropy, len);
  const char **str = (const char **)malloc((len+1) * sizeof(char *));
  for (int i = 0; i <= len; i++) {
    unsigned int idx = extract_index(entropy_with_checksum, i);
    str[i] = word_from_index(idx);
  }
  return str;
}

int word_to_index(const char *word) {
  for (int i = 0; i < BIP39_NUMBER_OF_WORDS; i++) {
    char tmp_word[40];
    bip39_word_at_index(tmp_word, i);
    if (strcmp(word, tmp_word) == 0) {
      return i;
    }
  }
  return -1;
}

const unsigned char *words_to_bytes(char **words, unsigned int words_len) {
  unsigned char *bytes = (unsigned char *)malloc(words_len * WORD_BITS / 8 + 1);

  for (int wi = 0, pos = 0; wi < words_len; wi++) {
    int found = word_to_index(words[wi]);
    for (int b = 0; b < WORD_BITS; b++, pos++) {
      bytes[pos / 8] |= (found >> (WORD_BITS - 1 - b)) << (7 - (pos % 8));
    }
  }

  return bytes;
}
