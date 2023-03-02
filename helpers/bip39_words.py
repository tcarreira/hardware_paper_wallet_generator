#!/usr/bin/env python3

import os
import pathlib
import sys

import requests

words_url = "https://raw.githubusercontent.com/bitcoin/bips/master/bip-0039/english.txt"
script_dir = pathlib.Path(__file__).parent.absolute()
cfile_name = "bip39_words"
destination_file_h = os.path.join(script_dir, "..", f"{cfile_name}.h")
destination_file_c = os.path.join(script_dir, "..", f"{cfile_name}.c")


def main():
    response = requests.get(words_url)
    if response.status_code != 200:
        print("Failed to download BIP39 word list")
        sys.exit(1)

    words = response.text.strip().splitlines()
    if len(words) != 2048:
        print("Invalid word list length")
        sys.exit(1)

    with open(destination_file_h, "w") as f:
        f.write("""/*This file was generated by helpers/%s
    DO NOT EDIT MANUALLY */
#ifndef HPWG_BIP39_WORDS_H
#define HPWG_BIP39_WORDS_H

#ifdef __cplusplus
extern "C" {
#endif

#define BIP39_NUMBER_OF_WORDS 2048

void bip39_word_at_index(char* dest_str, unsigned int index);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // HPWG_BIP39_WORDS_H
""" % os.path.basename(__file__))
        print("Saving file to %s" % destination_file_h)

    with open(destination_file_c, "w") as f:
        f.write("""/*  This file was generated by helpers/%s
    Downloaded from %s
    DO NOT EDIT MANUALLY */
// clang-format off

#include "bip39_words.h"
#include <stdlib.h>
#include <string.h>

const char* BIP39_WORDS[BIP39_NUMBER_OF_WORDS] = {
""" % (os.path.basename(__file__), words_url))

        for word in words[:-1]:
            f.write('  "%s",\n' % word)
        f.write('  "%s"' % words[-1])
        f.write("""
};

void bip39_word_at_index(char* dest_str, unsigned int index){
  if (index >= BIP39_NUMBER_OF_WORDS) {
    dest_str[0] = '\\0';
  } else {
    strcpy(dest_str, BIP39_WORDS[index]);
  }
  return;
}
""")
        print("Saving file to %s" % destination_file_c)


if __name__ == "__main__":
    main()
