/*  This file was generated by helpers/bip39_test_vectors.py
    DO NOT EDIT MANUALLY */
#pragma once
#define BIP39_VECTOR_COUNT 24

int vectorEntropyBytesLengthAtIndex(unsigned int index);
int vectorEntropyBitsAtIndex(unsigned int index);
void vectorEntropyStrAtIndex(char *dest_str, unsigned int index);
void vectorEntropyAtIndex(unsigned char *dest_bytes, unsigned int index);
int vectorWordsCountAtIndex(unsigned int index);
void vectorWordAtIndex(char *dest_str, unsigned int inde, unsigned int word_numx);
void vectorSeedAtIndex(char *dest_str, unsigned int index);
void vectorXprvAtIndex(char *dest_str, unsigned int index);
