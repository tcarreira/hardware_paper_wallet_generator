#include <assert.h>
#include <string.h>
#include "bip39_words.h"
#include "bip39_test_vectors.h"
#include "bip39.c"

void test_bip39_words_count() {
  assert(BIP39_NUMBER_OF_WORDS == 2048);
}

void test_bip39_get_word_index() {
  assert(strcmp(word_from_index(0), "abandon") == 0);
  assert(strcmp(word_from_index(255), "cable") == 0);
  assert(strcmp(word_from_index(999), "language") == 0);
  assert(strcmp(word_from_index(2047), "zoo") == 0);
}


void test_bip39() {
  test_bip39_words_count();
  test_bip39_get_word_index();
}
