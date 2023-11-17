#include "libbase58.h"
#include "mbedtls/md.h"
#include "sha-256.h"

void hmac_sha512_setup() {

  Serial.begin(115200);

  char *key = "secretKey";
  // char *payload = "Hello HMAC SHA 512!";
  char payload[] = {0x67, 0xc2, 0x0c, 0x6f, 0xc2, 0x72, 0x35, 0x38, 0xaf, 0x00, 0x9e, 0x0a, 0x84, 0xd5, 0x81, 0xf1, 0xce, 0xbf, 0xf3, 0xcd, 0x4b, 0x53, 0x28, 0xdd, 0x85, 0x59, 0xa5, 0xab, 0xe5, 0x13, 0xc7, 0xf6, 0x9f, 0xa1, 0xb9, 0x4f, 0xee, 0x88, 0x1e, 0x66, 0xc2, 0x4f, 0xcd, 0xdb, 0x7c, 0x5c, 0x5a, 0x5a, 0x3b, 0x0e, 0xf1, 0x32, 0xc3, 0x9c, 0x49, 0x16, 0xcd, 0xcb, 0x67, 0x77, 0xc0, 0xa3, 0xda, 0xd0, 0x00};
  byte hmacResult[64];

  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA512;

  const size_t payloadLength = strlen(payload);
  const size_t keyLength = strlen(key);

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char *)key, keyLength);
  mbedtls_md_hmac_update(&ctx, (const unsigned char *)payload, payloadLength);
  mbedtls_md_hmac_finish(&ctx, hmacResult);
  mbedtls_md_free(&ctx);

  Serial.print("Hash: ");

  for (int i = 0; i < sizeof(hmacResult); i++) {
    char str[3];

    sprintf(str, "%02x", (int)hmacResult[i]);
    Serial.print(str);
  }
  Serial.println();
  Serial.println("Hash: "
                 "c3e1003e1f58812dae5872fcfa682d1ad3f606b7c2f46233b5c05b4cb3a806016c22fcc720889e75140c4d9c6af8d50de3492"
                 "a76e6ac3a9835bddc8b45d432c1 (expected)");

  // Test b58 -> /////////////////////////////////
  size_t b58sz = 1024;
  char b58[b58sz];
  memset(b58, 0, b58sz);
  uint8_t data[] = {0x00, 0x5a, 0x1f, 0xc5, 0xdd, 0x9e, 0x6f, 0x03, 0x81, 0x9f, 0xca,
                    0x94, 0xa2, 0xd8, 0x96, 0x69, 0x46, 0x96, 0x67, 0xf9, 0xa0};
  bool result = b58enc(b58, &b58sz, data, 21);

  Serial.print("Base58: ");
  for (int i = 0; i < b58sz; i++) {
    Serial.print(b58[i]);
  }
  Serial.println();
  Serial.println("Base58: 12FpmoFq5cpWVRp4dCgkYB3HiTzx7  (expected)");
  // <- Test b58 /////////////////////////////////

  // 4 byte: version bytes (mainnet: 0x0488B21E public, 0x0488ADE4 private; testnet: 0x043587CF public, 0x04358394
  // private) 1 byte: depth: 0x00 for master nodes, 0x01 for level-1 derived keys, .... 4 bytes: the fingerprint of the
  // parent's key (0x00000000 if master key) 4 bytes: child number. This is ser32(i) for i in xi = xpar/i, with xi the
  // key being serialized. (0x00000000 if master key) 32 bytes: the chain code 33 bytes: the public key or private key
  // data (serP(K) for public keys, 0x00 || ser256(k) for private keys)

  unsigned char ser[78];
  ser[0] = 0x04; // 4 byte: version bytes
  ser[1] = 0x88;
  ser[2] = 0xb2;
  ser[3] = 0x1e;
  ser[4] = 0x00; // 1 byte: depth: 0x00
  ser[5] = 0x00; // 4 bytes: the fingerprint of the parent's key
  ser[6] = 0x00;
  ser[7] = 0x00;
  ser[8] = 0x00;
  ser[9] = 0x00; // 4 bytes: child number
  ser[10] = 0x00;
  ser[11] = 0x00;
  ser[12] = 0x00;
  for (int i = 13, j = 0; j < 32; i++, j++) {
    ser[i] = hmacResult[32 + j]; // 32 bytes: the chain code
  }
  ser[45] = 0x00;
  for (int i = 46, j = 0; j < 32; i++, j++) {
    ser[i] = hmacResult[j]; // 32 bytes: the public key or private key data
  }

  unsigned char myHash1[32];
  unsigned char myHash2[32];
  calc_sha_256(myHash1, ser, 78);
  calc_sha_256(myHash2, myHash1, 32);

  unsigned char to_encode[32 + 78];
  memcpy(&to_encode[0], myHash2, 32);
  memcpy(&to_encode[32], ser, 78);

  size_t b58sz_2 = 1024;
  char b58_2[b58sz_2];
  memset(b58_2, 0, b58sz_2);
  bool result2 = b58enc(b58_2, &b58sz_2, to_encode, 32 + 78);

  Serial.print("Base58: ");
  for (int i = 0; i < b58sz_2; i++) {
    Serial.print(b58_2[i]);
  }
  Serial.println();
  Serial.println("Base58: 12FpmoFq5cpWVRp4dCgkYB3HiTzx7  (expected)");
}

void hmac_sha512_loop() {
}
