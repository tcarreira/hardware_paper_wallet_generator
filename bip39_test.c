#include <assert.h>
#include "bip39_words.h"
#include "bip39_test_vectors.h"

void test_bip39_words_count() {
  assert(BIP39_NUMBER_OF_WORDS == 2048);
}

void test_bip39() {
  test_bip39_words_count();
}
