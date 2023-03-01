#include "bip39_test.h"
#include "bip39.h"
#include "bip39_test_vectors.h"
#include "bip39_words.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void test_bip39_word_from_index() {
  assert(BIP39_NUMBER_OF_WORDS == 2048);
}

void test_number_of_words_from_entropy_size() {
  assert(number_of_words_from_entropy_size(128) == 12);
  assert(number_of_words_from_entropy_size(160) == 15);
  assert(number_of_words_from_entropy_size(192) == 18);
  assert(number_of_words_from_entropy_size(224) == 21);
  assert(number_of_words_from_entropy_size(256) == 24);
}

void test_bip39_get_word_index() {
  assert(strcmp(word_from_index(0), "abandon") == 0);
  assert(strcmp(word_from_index(255), "cable") == 0);
  assert(strcmp(word_from_index(999), "language") == 0);
  assert(strcmp(word_from_index(2047), "zoo") == 0);
}

void test_bip39_entropy_to_words() {
  for (int test_num = 0; test_num < BIP39_VECTOR_COUNT; test_num++) {
    unsigned char *entropy = (unsigned char *)malloc(vectorEntropyBytesLengthAtIndex(test_num));
    vectorEntropyAtIndex(entropy, test_num);

    const char **words = entropy_to_words(entropy, vectorEntropyBytesLengthAtIndex(test_num));

    for (int i = 0; i < vectorWordsCountAtIndex(test_num); i++) {
      char word[40];
      vectorWordAtIndex(word, test_num, i);
      assert(strcmp(words[i], word) == 0);
    }
  }
}

void test_bip39_words_to_bytes() {
  for (int test_num = 1; test_num < BIP39_VECTOR_COUNT; test_num++) {
    char** words = (char**)malloc(vectorWordsCountAtIndex(test_num) * sizeof(char*));
    for (int i = 0; i < vectorWordsCountAtIndex(test_num); i++) {
      words[i] = (char*)malloc(40 * sizeof(char));
      vectorWordAtIndex(words[i], test_num, i);
    }
    const unsigned char *bytes = words_to_bytes(words, vectorWordsCountAtIndex(test_num));

    unsigned char *vector_entropy = (unsigned char *)malloc(vectorEntropyBytesLengthAtIndex(test_num));
    vectorEntropyAtIndex(vector_entropy, test_num);
    for (int i = 0; i < vectorEntropyBytesLengthAtIndex(test_num); i++) {
      assert(bytes[i] == vector_entropy[i]);
    }
  }
}

void test_bip39() {
  test_bip39_word_from_index();
  test_number_of_words_from_entropy_size();
  test_bip39_get_word_index();
  test_bip39_entropy_to_words();
  test_bip39_words_to_bytes();
}
