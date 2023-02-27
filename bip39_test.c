#include "bip39.c"
#include "bip39_test_vectors.h"
#include "bip39_words.h"
#include <assert.h>
#include <string.h>

void test_bip39_word_from_index() {
  assert(BIP39_NUMBER_OF_WORDS == 2048);
}

void test_bip39_get_word_index() {
  assert(strcmp(word_from_index(0), "abandon") == 0);
  assert(strcmp(word_from_index(255), "cable") == 0);
  assert(strcmp(word_from_index(999), "language") == 0);
  assert(strcmp(word_from_index(2047), "zoo") == 0);
}

void test_bip39_entropy_to_words() {
  for (int test_num = 0; test_num < BIP39_VECTOR_COUNT; test_num++) {
    unsigned char *entropy = (unsigned char *)vectorEntropy[test_num];
    const char **words = entropy_to_words(entropy, vectorEntropyBytesLength[test_num]);

    for (int i = 0; i < vectorWordsCount[test_num]; i++) {
      assert(strcmp(words[i], vectorWords[test_num][i]) == 0);
    }
  }
}

void test_bip39_words_to_bytes() {
  for (int test_num = 1; test_num < BIP39_VECTOR_COUNT; test_num++) {
    const unsigned char *bytes = words_to_bytes(vectorWords[test_num], vectorWordsCount[test_num]);

    for (int i = 0; i < vectorEntropyBytesLength[test_num]; i++) {
      assert(bytes[i] == vectorEntropy[test_num][i]);
    }
  }
}

void test_bip39() {
  test_bip39_word_from_index();
  test_bip39_get_word_index();
  test_bip39_entropy_to_words();
  test_bip39_words_to_bytes();
}
