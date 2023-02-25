#include "bip39.h"
#include "bip39_words.h"

const char* word_from_index(int idx){
  return BIP39_WORDS[idx];
}
