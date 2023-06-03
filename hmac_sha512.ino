#include "mbedtls/md.h"

void hmac_sha512_setup() {

  Serial.begin(115200);

  char *key = "secretKey";
  char *payload = "Hello HMAC SHA 512!";
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
  Serial.println("Hash: c3e1003e1f58812dae5872fcfa682d1ad3f606b7c2f46233b5c05b4cb3a806016c22fcc720889e75140c4d9c6af8d50de3492a76e6ac3a9835bddc8b45d432c1 (expected)");

}

void hmac_sha512_loop() {
}
