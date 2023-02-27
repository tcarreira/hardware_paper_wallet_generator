#include <stdlib.h>
#include <stdio.h>
#include "bip39.h"
#include "bip39_words.h"

#define WORD_BITS 11

const char *word_from_index(int idx)
{
    return BIP39_WORDS[idx];
}

unsigned int extract_index(const unsigned char *bytes, unsigned int n)
{
    unsigned int value = 0;
    unsigned int end = n * WORD_BITS + WORD_BITS;
    for (unsigned int pos = n * WORD_BITS; pos < end; ++pos)
    {
        unsigned char byte_at = bytes[pos / 8u];
        unsigned char mask = 1u << (7u - (pos % 8u));
        value = (value << 1u) | !!(byte_at & mask);
    }
    return value;
}

const char **entropy_to_words(const unsigned char *entropy, unsigned int len)
{
    // const unsigned char * entropy_with_checksum = entropy_checksum(entropy, len);

    const char **str = malloc(len * sizeof(char *));
    for (int i = 0; i < len; i++)
    {
        unsigned int idx = extract_index(entropy, i);
        str[i] = word_from_index(idx);
    }
    return str;
}
