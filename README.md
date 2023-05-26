# Hardware Paper Wallet for Bitcoin

A hardware device, 100% offline and air-gapped, for generating a paper wallet.
This is intended for HODL only, not for using the wallet's funds.

## Motivation

Having a 99.999% secure Bitcoin wallet, by generating it offline, with a 100% free and open sourced code.
This device shall produce the following:

- Hardware mnemonic (words for backing up the wallet)
- Master public Key
- Some addresses for transfering funds.

When you are ready to withdraw funds from the wallet, you should perform the following steps:

- Use [some software wallet](https://bitcoin.org/en/choose-your-wallet) (eg: Electrum)
- recover wallet (from mnemonics)
- use your funds
- Secure your remaining funds
  - Create a new offline paper wallet with the hardware
  - Transfer everything from the old wallet (it has possibly been compromised, no longer offline) to one of the newest addresses (and do not use it again, for privacy)
  - Your funds are secure now

## Hardware workflow

WIP - Work in Progress.
(disclaimer: if any bullet is not complete yet, do not use this!)

- [ ] Generate true random Entropy
- [x] Create a Seed
- [x] Convert seed to mnemonic words
- [ ] Show Master Public Key (xpub)
- [ ] Sub-wallet Derivation (m/84'/0'/0')
  - [ ] Show Public Key (xpub)
  - [ ] Show Addresses
