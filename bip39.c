#include "bip39.h"
#include "WjCryptLib/lib/WjCryptLib_Sha256.c"
#include "bip39_words.h"
#include <stdio.h>
#include <stdlib.h>

#define WORD_BITS 11

const char *word_from_index(int idx) {
  return BIP39_WORDS[idx];
}

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
  Sha256Context sha256Context;
  SHA256_HASH sha256Hash;

  // Calculate sha256 hash
  Sha256Initialise(&sha256Context);
  Sha256Update(&sha256Context, entropy, len);
  Sha256Finalise(&sha256Context, &sha256Hash);

  // Get most significant bits of hash. 128->4, 160->5, 192->6, 224->7, 256->8
  unsigned int needed_bits = len * 8 / 32;
  unsigned char mask = 0xff << (8 - needed_bits);

  // Append most significant bits of hash to entropy
  unsigned char *entropy_with_checksum = malloc(len + 1);
  memcpy(entropy_with_checksum, entropy, len);
  entropy_with_checksum[len] = sha256Hash.bytes[0] & mask;

  return entropy_with_checksum;
}

const char **entropy_to_words(const unsigned char *entropy, unsigned int len) {
  const unsigned char *entropy_with_checksum = entropy_checksum(entropy, len);
  const char **str = malloc(len * sizeof(char *));
  for (int i = 0; i < len; i++) {
    unsigned int idx = extract_index(entropy_with_checksum, i);
    str[i] = word_from_index(idx);
  }
  return str;
}
